#pragma once 

//#include <Serialisation/Stream.h>
#include "Util/Logger.h"
#include <Base/MemberTraits.h>
#include <Serialisation/JsonStreamWriter.h>
#include <Interface/ISerialisable.h>
#include <CCCommon.h>
#include <memory>
#include <nameof.hpp>
#include <string>
#include <type_traits>
#include <typeindex>

namespace CitrusCore 
{
	template <auto Member>
	constexpr auto member_name = NAMEOF_MEMBER(Member);

    struct CITRUS_CORE_API PropertyData 
    {
        std::string Name;
        std::string Type;
        std::string value;
    };

    template<typename T>
    struct is_json_compatible : std::false_type {};

    template<>
    struct is_json_compatible<int> : std::true_type {};

    template<>
    struct is_json_compatible<float> : std::true_type {};

    template<>
    struct is_json_compatible<double> : std::true_type {};

    template<>
    struct is_json_compatible<bool> : std::true_type {};

    template<>
    struct is_json_compatible<std::string> : std::true_type {};

    template<typename T>
    struct is_json_compatible<std::vector<T>> : is_json_compatible<T> {};

    template<typename T>
    struct is_json_compatible<std::optional<T>> : is_json_compatible<T> {};

    template <typename T>
    using pointer_traits_element_t = typename std::pointer_traits<T>::element_type;

    template <typename T>
    struct is_smart_pointer_impl : std::false_type {};

    template <typename T>
    struct is_smart_pointer_impl<std::shared_ptr<T>> : std::true_type {};

    template <typename T>
    struct is_smart_pointer_impl<std::unique_ptr<T>> : std::true_type {};

    template <typename T>
    struct is_smart_pointer : is_smart_pointer_impl<std::remove_cv_t<std::remove_reference_t<T>>> {};

    class CITRUS_CORE_API PropertyBase : ISerialisable
    {
    public:
        ~PropertyBase() = default;
        PropertyBase() = default;
        virtual void Serialise(StreamWriter& byteStream) override {}
        virtual void Deserialise(StreamReader& byteStream) override {}
        bool IsReadOnly() const { return m_bReadOnly; }
    protected:\
        std::string m_propertyName;
        std::string m_typeName;
        bool m_bReadOnly = true;  
        std::type_index m_type;
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API PropertyBaseT : public PropertyBase
    {
    public:
        PropertyBaseT(Base* baseClass, FieldType Base::* member)
        {
            using fieldTrait = typename CitrusCore::MemberTraits<decltype(Member)>::field_type;
            m_propertyName = std::string(member_name<Member>);
            m_typeName = std::string(NAMEOF_TYPE(fieldTrait));
            m_obj = baseClass;
            m_property = member;
        }
        const FieldType& Get() const requires(m_bReadOnly) { return m_obj.*m_property; }
        FieldType& Get() const requires(!m_bReadOnly) { return m_obj.*m_property; }

        virtual void Deserialise(StreamReader& stream) override
        {
            // Pointless as the name does not change
            //m_propertyName = stream.ReadString("Name", "m_unknownProperty"); 
            auto& value = m_obj->*m_property;
            using BareType = std::remove_pointer_t<FieldType>;

            if constexpr (is_smart_pointer<FieldType>::value)
            {
                using fieldBareType = pointer_traits_element_t<FieldType>();
                if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, fieldBareType>) {
                    value->Deserialise(stream);
                }
            }
            else if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, FieldType>)
            {
                value.Deserialise(stream);
            }
            else if constexpr (is_json_compatible<BareType>::value)
            {
                bool isSignedInt = std::string(NAMEOF_TYPE(int)) == m_typeName ||
                                std::string(NAMEOF_TYPE(int32_t)) == m_typeName;
            
                bool isUnsignedInt = std::string(NAMEOF_TYPE(uint32_t)) == m_typeName;
                                
                bool isFloat = std::string(NAMEOF_TYPE(float)) == m_typeName;
                bool isString = std::string(NAMEOF_TYPE(std::string)) == m_typeName;

                if (isSignedInt)
                {
                    if constexpr (std::is_pointer_v<FieldType>()) {
                        *value = stream.ReadSigned("Value");
                    }
                    else {
                        value = stream.ReadSigned("Value");
                    }
                }
                else if (isUnsignedInt)
                {
                    if constexpr (std::is_pointer_v<FieldType>()) {
                        *value = stream.ReadUnsigned("Value");
                    }
                    else {
                        value = stream.ReadUnsigned("Value");
                    }
                }
                else if (isFloat)
                {
                    if constexpr (std::is_pointer_v<FieldType>()) {
                        *value = stream.ReadFloat("Value", *value);
                    }
                    else {
                        value = stream.ReadFloat("Value", value);
                    }
                }
                else if (isString)
                {
                    if constexpr (std::is_pointer_v<FieldType>()) {
                        *value = stream.ReadString("Value", *value);
                    }
                    else {
                        value = stream.ReadString("Value", value);
                    }
                }
                else{
                    Logger::Log(Logger::ERROR, "Error deserialising property, unsupported type [%s].", m_typeName.c_str());
                }
            }
        }

        virtual void Serialise(StreamWriter& stream) override
        {
            stream.BeginObject("Property");
            stream.Write("Name", m_propertyName);
            stream.Write("Type", m_typeName);

            using BareType = std::remove_pointer_t<FieldType>;

            //using underlying_type = pointer_traits_element_t<FieldType>;
            auto& value = m_obj->*m_property;

            if constexpr (is_smart_pointer<FieldType>::value)
            {
                using fieldBareType = pointer_traits_element_t<FieldType>();
                if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, fieldBareType>) {
                    value->Serialise(stream);
                }
            }
            else if constexpr (std::is_base_of_v<CitrusCore::ISerialisable, FieldType>)
            {
                value.Serialise(stream);
            }
            else if constexpr (is_json_compatible<BareType>::value)
            {
                if constexpr (std::is_pointer_v<FieldType>()) {
                    stream.Write("Value", json(*value));
                }
                else {
                    stream.Write("Value", json(value));
                }
            }
            stream.EndObject();
        }
    protected:
        Base* m_obj = nullptr;
        FieldType Base::* m_property;
    };

    template <class FieldType>
    class CITRUS_CORE_API IPropertyT{
    public:
        virtual void Set(FieldType& value) = 0;
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API PropertyT : public PropertyBaseT<Base, FieldType, Member>, public IPropertyT<FieldType>
    {
    public:
        ~PropertyT() = default;
        PropertyT(Base* baseClass, FieldType Base::* member) : PropertyBaseT<Base, FieldType, Member>(baseClass, member){}
        virtual void Set(FieldType& value) override { this->m_obj->*this->m_property = value;}
    };

    template <class Base, class FieldType, FieldType Base::* Member>  
    class CITRUS_CORE_API ReadOnlyPropertyT : public PropertyBaseT<Base, FieldType, Member>{
    public:
        ReadOnlyPropertyT(Base* baseClass, FieldType Base::* member) : PropertyBaseT<Base, FieldType, Member>(baseClass, member)
        {
            this->m_bReadOnly = true;
        }
    };
}

