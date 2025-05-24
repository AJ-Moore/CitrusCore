#pragma once

#include <CCCommon.h>
#include <Types.h>
#include <cstddef>
#include <cstring>
#include <vector>

namespace CitrusCore 
{
    class CITRUS_CORE_API ByteStream 
    {
    public:
        void Reserve(long int sizeInBytes);

        template <class T>
        std::enable_if<std::is_integral<T>::value, void>
        WriteInt(T value);

        //void WriteUnsigned(uint64 value, int bytes);
        //void WriteSigned(sint64 value, int bytes);
        void WriteString(std::string value);
        void WriteBool(bool value);
        void WriteFloat(float value);
    private:
        std::vector<uint8> m_byteStream;
        int m_byteIndex = 0;
        int m_readIndex = 0;
    };

    template <class T>
    std::enable_if<std::is_integral<T>::value, void>
    ByteStream::WriteInt(T value)
    {
        size_t sizeInBytes = sizeof(T);

        if (m_byteIndex + sizeInBytes >= m_byteStream.capacity())
        {
            m_byteStream.resize(m_byteIndex + sizeInBytes);
        }

        std::memcpy(&m_byteStream[m_byteIndex], value, sizeInBytes);
    }
}