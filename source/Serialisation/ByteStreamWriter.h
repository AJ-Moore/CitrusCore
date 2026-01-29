#pragma once

#include <CCCommon.h>
#include <Types.h>
#include <cstddef>
#include <cstring>
#include <vector>
#include <Serialisation/Stream.h>

namespace CitrusCore 
{
    class CITRUS_CORE_API ByteStreamWriter : public StreamWriter
    {
    public:
        void Reserve(long int sizeInBytes);

        template <class T>
        std::enable_if<std::is_integral<T>::value, void>
        WriteInt(T value);

        virtual void Write(const std::string& key, uint64 value) override;
        virtual void Write(const std::string& key, sint64 value) override;
        virtual void Write(const std::string& key, std::string value) override;
        virtual void Write(const std::string& key, bool value) override;
        virtual void Write(const std::string& key, float value) override;
    private:
        std::vector<uint8> m_byteStream;
        int m_byteIndex = 0;
        int m_readIndex = 0;
    };

    template <class T>
    std::enable_if<std::is_integral<T>::value, void>
    ByteStreamWriter::WriteInt(T value)
    {
        size_t sizeInBytes = sizeof(T);

        if (m_byteIndex + sizeInBytes >= m_byteStream.capacity())
        {
            m_byteStream.resize(m_byteIndex + sizeInBytes);
        }

        std::memcpy(&m_byteStream[m_byteIndex], &value, sizeInBytes);
    }
}