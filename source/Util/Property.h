#pragma once 

#include <Interface/ISerialisable.h>
#include <CCCommon.h>

namespace CitrusCore {
    template <class T>
    class CITRUS_CORE_API Property : ISerialisable
    {
    public:
        T Get() const { return m_value; }
        virtual void Serialise(ByteStream& byteStream) override = 0;
        virtual void Deserialise(ByteStream& byteStream) override = 0;
    private:
        T m_value;
    };
}

