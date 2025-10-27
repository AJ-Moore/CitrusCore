#pragma once 

#include <Interface/ISerialisable.h>
#include <CCCommon.h>

namespace CitrusCore 
{
    class CITRUS_CORE_API PropertyBase : ISerialisable
    {
    public:
        PropertyBase() = default;
        virtual void Serialise(ByteStream& byteStream) override = 0;
        virtual void Deserialise(ByteStreamReader& byteStream) override = 0;
        bool IsReadOnly() const { return m_bReadOnly; }
    protected:
        std::string m_name;
        bool m_bReadOnly = true;  
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API SerialisedProperty : public PropertyBase
    {
    public:
        SerialisedProperty(Base* baseClass, FieldType Base::* member)
        {
            m_name = nameof_member(member);
        }
        const FieldType& Get() const { return m_obj.*m_property; }
    protected:
        Base* m_obj = nullptr;
        FieldType Base::* m_property;
    };

    template <class Base, class FieldType, FieldType Base::* Member>
    class CITRUS_CORE_API Property : public SerialisedProperty<Base, FieldType, Member>
    {
    public:
        Property(Base* baseClass, FieldType Base::* member) : SerialisedProperty<Base, FieldType, Member>(baseClass, member){}
        void Set(FieldType& value){ this->m_obj->*this->m_property = value;}
    };

    template <class Base, class FieldType, FieldType Base::* Member>  
    class CITRUS_CORE_API ReadOnlyProperty : public Property<Base, FieldType, Member>{
        ReadOnlyProperty(Base* baseClass, FieldType Base::* member) : Property<Base, FieldType, Member>(baseClass, member)
        {
            this->m_bReadOnly = true;
        }
    };
}

