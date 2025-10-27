#pragma once

#include "Util/Property.h"
#include <Util/UID.h>
#include <cstddef>
#include <memory>
#include <nameof.hpp>
#include <unordered_map>

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
		requires std::is_member_object_pointer_v<decltype(Member)>
		void RegisterProperty(T* obj, bool readOnly = false);

		template <auto Member>
		requires std::is_member_object_pointer_v<decltype(Member)>
		const auto* GetProperty();
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

	template <auto Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	const auto* Object::GetProperty()
	{
		std::string propertyName = nameof_member(Member);
		auto found = m_properties.find(propertyName);

		using FieldType = typename member_traits<decltype(Member)>::field_type;
		using ClassType = typename member_traits<decltype(Member)>::class_type;

		if (found != m_properties.end())
		{
			if (found->second->IsReadOnly())
			{
				return static_cast<ReadOnlyProperty<ClassType,FieldType,Member>*>(found->second.get());
			}
			else {
				return static_cast<SerialisedProperty<ClassType,FieldType,Member>*>(found->second.get());
			}
		}

		return nullptr;
	}

	template <class T, auto Member>
	requires std::is_member_object_pointer_v<decltype(Member)>
	void Object::RegisterProperty(T* obj, bool readOnly)
	{
		using FieldType = typename member_traits<decltype(Member)>::field_type;
		using ClassType = typename member_traits<decltype(Member)>::class_type;
	
		static_assert(std::is_same_v<T, ClassType>, "Object type mismatch");

		if (readOnly)
		{
			RegisterProperty(std::make_unique<SerialisedProperty<T, FieldType, Member>>(obj, Member), Member);
		}
		else {
			RegisterProperty(std::make_unique<ReadOnlyProperty<T, FieldType, Member>>(obj, Member), Member);
		}
	}
}