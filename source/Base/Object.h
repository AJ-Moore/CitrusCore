#pragma once

#include <Base/IPropertyRenderer.h>
#include <Base/PropertyDefinitionBase.h>
#include <Reflection/Reflect.h>
#include <Interface/ISerialisable.h>
#include <Serialisation/Stream.h>
#include <Util/Logger.h>
#include <CCCommon.h>
#include <Util/Property.h>
#include <Util/UID.h>
#include <glm/fwd.hpp>
#include <memory>
#include <nameof.hpp>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <ranges>
#include <Base/MemberTraits.h>

namespace CitrusCore
{
	/** Base object class. */
	class CITRUS_CORE_API Object
	{
	public:
		virtual ~Object() = default;

		Object() = default;

        Object(Object&& other) noexcept
            : m_properties(std::move(other.m_properties))
        {}

        Object& operator=(Object&& other) noexcept {
            m_properties = std::move(other.m_properties);
            return *this;
		}

		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName() const { return m_name; }
		const UID& GetUID() const { return m_uid; }

		template <class T, class FieldType, FieldType T::* member>
		void RegisterProperty(std::unique_ptr<PropertyBase> property, FieldType T::* classProp);

		template <class T, auto Member>
		requires std::is_member_object_pointer_v<decltype(Member)>
		void RegisterProperty(T* obj, bool readOnly = false);

		template <auto Member>
		requires std::is_member_object_pointer_v<decltype(Member)>
		auto* GetProperty();

		template <auto Member, class T>
		requires std::is_member_object_pointer_v<decltype(Member)>
		auto GetProperty(T* obj);

		PropertyBase* GetProperty(std::string) const;

		/// returns an std::views of property value ptr's > Allows one to iterate through property values
		auto PropertyValues() const {
			// Return view of references to unique_ptr
			return m_properties | std::views::values | std::views::transform([](auto& p) -> PropertyBase* { 
				return p.get(); 
			});
		}
	private:
		UID m_uid;
		std::string m_name = "Object";

		std::unordered_map<std::string, std::unique_ptr<PropertyBase>> m_properties;
	};

	template <class T, class FieldType, FieldType T::* member>
	void Object::RegisterProperty(std::unique_ptr<PropertyBase> property, FieldType T::* classProp)
    {
        m_properties[nameof::nameof_member(classProp)] = property;
    }

	/// Property must be registered in m_properties to work, additional lookup cost.
	template <auto Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	auto* Object::GetProperty()
	{
		std::string propertyName = std::string(member_name<Member>);
		auto found = m_properties.find(propertyName);

		using FieldType = typename MemberTraits<decltype(Member)>::field_type;
		using ClassType = typename MemberTraits<decltype(Member)>::class_type;

		if constexpr (std::is_base_of<IPropReadOnly, FieldType>::value)
		{
			return static_cast<ReadOnlyPropertyT<ClassType,FieldType,Member>*>(found->second.get());
		} else if constexpr (std::is_base_of<PropertyDefinitionBase, FieldType>::value) {
			return static_cast<PropertyT<ClassType,FieldType,Member>*>(found->second.get());
		}
		else {
			static_assert(false, "Property must be wrapped by Property or ReadOnlyProperty.");
		}
	}

	/// Property 'located' at compile time, no registeration required.
	template <auto Member, class T>
	requires std::is_member_object_pointer_v<decltype(Member)>
	auto Object::GetProperty(T* obj)
	{
		using FieldType = typename MemberTraits<decltype(Member)>::field_type;
		using ClassType = typename MemberTraits<decltype(Member)>::class_type;

		if constexpr (std::is_base_of<IPropReadOnly, FieldType>::value)
		{
			return ReadOnlyPropertyT<T, FieldType, Member>(obj, Member);
		} else if constexpr (std::is_base_of<PropertyDefinitionBase, FieldType>::value) {
			return PropertyT<T, FieldType, Member>(obj, Member);
		}
		else {
			static_assert(false, "Property must be wrapped by Property or ReadOnlyProperty.");
		}
	}

	template <class T, auto Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	void Object::RegisterProperty(T* obj, bool readOnly)
	{
		using FieldType = typename MemberTraits<decltype(Member)>::field_type;
		using ClassType = typename MemberTraits<decltype(Member)>::class_type;

		Logger::Log(Logger::VERBOSE, "Filed type [%s]",typeid(FieldType).name());
	
		static_assert(std::is_same_v<T, ClassType>, "Object type mismatch");

		if (readOnly)
		{
			std::string name = std::string(member_name<Member>);
			std::unique_ptr<ReadOnlyPropertyT<T, FieldType, Member>> property = std::make_unique<ReadOnlyPropertyT<T, FieldType, Member>>(obj, Member);
			m_properties[name] = std::move(property);
		}
		else {
			std::string name = std::string(member_name<Member>);
			std::unique_ptr<PropertyT<T, FieldType, Member>> property = std::make_unique<PropertyT<T, FieldType, Member>>(obj, Member);
			m_properties[name] = std::move(property);
		}
	}

	template <class T, bool ReadOnly = false>
    class CITRUS_CORE_API PropertyDefinition : public PropertyDefinitionBase, public ISerialisable
    {
	public:
		constexpr PropertyDefinition() : PropertyDefinitionBase(ReadOnly){}
		constexpr bool IsReadOnly() const { return m_readOnly; }

		// Copy constructor
		constexpr PropertyDefinition(const T& initial) : PropertyDefinitionBase(ReadOnly), m_value(initial) {
			m_typeId = TypeToId<T>().Id;
		}

		// Move constructor
		constexpr PropertyDefinition(T&& initial) : PropertyDefinitionBase(ReadOnly), m_value(std::move(initial)) {
			m_typeId = TypeToId<T>().Id;
		}
	
		operator const T&() const { return m_value; }
	
		void operator=(const T& newValue) {
			m_value = newValue;
			OnChange.Invoke(m_value);
		}

		// Move
		void operator=(const T&& newValue) {
			m_value = std::move(newValue);
		}
	
		const T& Get() const { return m_value; }
		TypeID GetTypeId() const { return m_typeId; }
		void Set(const T& newValue) { *this = newValue; }

		virtual void RenderUI(PropertyBase* property, IPropertyRenderer& renderer) override
		{
			if constexpr (is_smart_pointer<T>::value)
            {
				using fieldBareType = pointer_traits_element_t<T>;

				if constexpr (std::is_integral_v<fieldBareType> ||
					std::is_same_v<fieldBareType, bool> ||
					std::is_same_v<fieldBareType, float> ||
					std::is_same_v<fieldBareType, double> ||
					std::is_same_v<fieldBareType, glm::vec3> || 
					std::is_same_v<fieldBareType, glm::mat4> ||
					std::is_same_v<fieldBareType, Transform> ||
					std::is_base_of_v<Transform, fieldBareType>
				) {
					renderer.Draw(property, *m_value);
				}
			}

			if constexpr (std::is_integral_v<T> ||
				std::is_same_v<T, bool> ||
				std::is_same_v<T, float> ||
				std::is_same_v<T, double> ||
				std::is_same_v<T, glm::vec3> || 
				std::is_same_v<T, glm::mat4> ||
				std::is_same_v<T, Transform> ||
				std::is_base_of_v<Transform, T>
			) {
				renderer.Draw(property, m_value);
			}
		}

		virtual void Deserialise(StreamReader& stream) override
		{
			if constexpr (requires { m_value->Deserialise(stream); }) { 
				m_value->Deserialise(stream);
			}

            if constexpr (is_smart_pointer<T>::value)
            {
                using fieldBareType = pointer_traits_element_t<T>();
                if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, fieldBareType>) {
                    m_value->Deserialise(stream);
                }
            }
            else if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, T>)
            {
                m_value.Deserialise(stream);
            }
			else if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, T> || is_json_compatible<T>::value)
			{
				constexpr bool isSignedInt = std::is_integral_v<T> && std::is_signed_v<T>;
				constexpr bool isUnsignedInt = std::is_integral_v<T> && std::is_unsigned_v<T>;
				constexpr bool isFloat = std::is_same_v<T, float>;
				constexpr bool isString = std::is_same_v<T, std::string>;

				const std::string typeName = std::string(NAMEOF_TYPE(T));

				if constexpr (isSignedInt)
				{
					if constexpr (std::is_pointer_v<T>) {
						*m_value = stream.ReadSigned("Value");
					}
					else {
						m_value = stream.ReadSigned("Value");
					}
				}
				else if constexpr (isUnsignedInt)
				{
					if constexpr (std::is_pointer_v<T>) {
						*m_value = stream.ReadUnsigned("Value");
					}
					else {
						m_value = stream.ReadUnsigned("Value");
					}
				}
				else if constexpr (isFloat)
				{
					if constexpr (std::is_pointer_v<T>) {
						*m_value = stream.ReadFloat("Value", *m_value);
					}
					else {
						m_value = stream.ReadFloat("Value", m_value);
					}
				}
				else if constexpr (isString)
				{
					if constexpr (std::is_pointer_v<T>) {
						*m_value = stream.ReadString("Value", *m_value);
					}
					else {
						m_value = stream.ReadString("Value", m_value);
					}
				}
				else{
					Logger::Log(Logger::ERROR, "Error deserialising property, unsupported type [%s].", typeName.c_str());
				}
			}
		}

		virtual void Serialise(StreamWriter& stream) override{

			//static_assert(is_smart_pointer<T>::value, "Ptr should be a smart pointer");
			Logger::Log(Logger::VERBOSE, "Template type [%s]",typeid(T).name());

			if constexpr (requires { m_value->Serialise(stream); }) { 
				m_value->Serialise(stream);
			}

            if constexpr (is_smart_pointer<T>::value)
            {
                using fieldBareType = pointer_traits_element_t<T>();
				if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, fieldBareType>)
				{
					m_value->Serialise(stream);
                }
				else if constexpr (is_json_compatible<fieldBareType>::value)
				{
					stream.Write("Value", json(*m_value));
				}
            }
            else if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, T>)
            {
                m_value.Serialise(stream);
            }
            else if constexpr (is_json_compatible<T>::value)
            {
				stream.Write("Value", json(m_value));
            }
		}

		Event<T> OnChange;
	private:
		T m_value;
		TypeID m_typeId;
	};

	template <class T, bool ReadOnly = false>
	class CITRUS_CORE_API ReadOnlyPropertyDefinition : public PropertyDefinition<T, ReadOnly>, public IPropReadOnly{
	public:
		constexpr ReadOnlyPropertyDefinition() = default;
		constexpr ReadOnlyPropertyDefinition(const T& v) : PropertyDefinition<T, ReadOnly>(v) {}
	};

	template <class T, typename... Traits>
	using Property = PropertyDefinition<T, false>;

	template <class T>
	using ReadOnlyProperty = ReadOnlyPropertyDefinition<T, true>;
}