#pragma once 

#include <Interface/ISerialisable.h>
#include <CCCommon.h>
#include <nameof.hpp>
#include <string>

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

    class CITRUS_CORE_API PropertyBase : ISerialisable
    {
    public:
        ~PropertyBase() = default;
        PropertyBase() = default;
        virtual void Serialise(ByteStreamWriter& byteStream) override {}
        virtual void Deserialise(ByteStreamReader& byteStream) override {}
        bool IsReadOnly() const { return m_bReadOnly; }
    protected:
        std::string m_name;
        bool m_bReadOnly = true;  
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API PropertyBaseT : public PropertyBase
    {
    public:
        PropertyBaseT(Base* baseClass, FieldType Base::* member)
        {
            m_name = std::string(member_name<Member>);
        }
        const FieldType& Get() const requires(m_bReadOnly) { return m_obj.*m_property; }
        FieldType& Get() const requires(!m_bReadOnly) { return m_obj.*m_property; }
    protected:
        Base* m_obj = nullptr;
        FieldType Base::* m_property;
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API PropertyT : public PropertyBaseT<Base, FieldType, Member>
    {
    public:
        ~PropertyT() = default;
        PropertyT(Base* baseClass, FieldType Base::* member) : PropertyBaseT<Base, FieldType, Member>(baseClass, member){}
        void Set(FieldType& value){ this->m_obj->*this->m_property = value;}
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

