#pragma once

#include "CCCommon.h"
#include "Util/Property.h"
#include <Util/UID.h>
#include <cstddef>
#include <memory>
#include <nameof.hpp>
#include <unordered_map>
#include <utility>

namespace CitrusCore
{
	/** Base object class. */
	class CITRUS_CORE_API Object
	{
	public:
		virtual ~Object(){}
		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName() const { return m_name; }
		const UID GetUID() const { return m_uid; }

		template <class T, class FieldType, FieldType T::* member>
		void RegisterProperty(std::unique_ptr<PropertyBase> property, FieldType T::* classProp);

		template <class T, auto Member>
		//template <class T, class FieldType, FieldType T::* Member>
		requires std::is_member_object_pointer_v<decltype(Member)>
		void RegisterProperty(T* obj, bool readOnly = false);

		template <auto Member>
		requires std::is_member_object_pointer_v<decltype(Member)>
		auto* GetProperty();
	private:
		UID m_uid;
		std::string m_name = "Object";

		std::unordered_map<std::string, std::unique_ptr<PropertyBase>> m_properties;
	};

	template <typename M> struct member_traits;
	template <typename T, typename FieldType>
	struct member_traits<FieldType T::*> {
		using class_type = T;
		using field_type = FieldType;
	};

	template <class T, class FieldType, FieldType T::* member>
	void Object::RegisterProperty(std::unique_ptr<PropertyBase> property, FieldType T::* classProp)
    {
        m_properties[nameof::nameof_member(classProp)] = property;
    }

	class CITRUS_CORE_API IPropReadOnly{};
	class CITRUS_CORE_API PropertyDefinitionBase{
	protected:
		bool m_readOnly = false;
	public:
		constexpr PropertyDefinitionBase(bool readOnly) : m_readOnly(readOnly) {}
		constexpr bool IsReadOnly() const { return m_readOnly; }
	};

	template <auto Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	auto* Object::GetProperty()
	{
		std::string propertyName = std::string(member_name<Member>);
		auto found = m_properties.find(propertyName);

		//if (found == m_properties.end())
		//{
		//	return nullptr;
		//}

		using FieldType = typename member_traits<decltype(Member)>::field_type;
		using ClassType = typename member_traits<decltype(Member)>::class_type;

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

	template <class T, auto Member>
	//template <class T, class FieldType, FieldType T::* Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	void Object::RegisterProperty(T* obj, bool readOnly)
	{
		using FieldType = typename member_traits<decltype(Member)>::field_type;
		using ClassType = typename member_traits<decltype(Member)>::class_type;
	
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
    class CITRUS_CORE_API PropertyDefinition : public PropertyDefinitionBase
    {
	public:
		constexpr PropertyDefinition() : PropertyDefinitionBase(ReadOnly){}
		constexpr bool IsReadOnly() const { return m_readOnly; }

		PropertyDefinition(const T& initial) : PropertyDefinitionBase(ReadOnly), m_value(initial) {}
	
		operator const T&() const { return m_value; }
	
		void operator=(const T& newValue) {
			m_value = newValue;
		}
	
		const T& Get() const { return m_value; }
		void Set(const T& newValue) { *this = newValue; }
	private:
		T m_value;
	};

	template <class T, bool ReadOnly = false>
	class CITRUS_CORE_API ReadOnlyPropertyDefinition : public PropertyDefinition<T, ReadOnly>, public IPropReadOnly{};

	template <class T>
	using Property = PropertyDefinition<T, false>;

	template <class T>
	using ReadOnlyProperty = ReadOnlyPropertyDefinition<T, true>;
}