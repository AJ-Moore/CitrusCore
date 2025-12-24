#include <Serialisation/ByteStreamWriter.h>
#include <cstddef>
#include <cstring>

namespace CitrusCore
{
    void ByteStreamWriter::Write(const std::string& key, std::string value)
    {
        if (!value.size())
        {
            return;
        }

        int remaining = m_byteStream.capacity() - m_byteIndex;
        size_t sizeInBytes = sizeof(value);

        if (remaining <= sizeInBytes)
        {
            int byteIndex = m_byteStream.capacity() - remaining;
            m_byteStream.resize(byteIndex + value.size());
        }

        std::memcpy(&m_byteStream[m_byteIndex], &value[0], sizeInBytes);
        m_byteIndex += sizeInBytes;
    }
    
    void ByteStreamWriter::Write(const std::string& key, bool value)
    {
        if (m_byteIndex + sizeof(bool) >= m_byteStream.capacity())
        {
            m_byteStream.resize(m_byteIndex + sizeof(bool));
        }

        m_byteStream[m_byteIndex] = value;
        m_byteIndex++;
    }

    void ByteStreamWriter::Write(const std::string&, float value)
    {
        if (m_byteIndex + sizeof(float) >= m_byteStream.capacity())
        {
            m_byteStream.resize(m_byteIndex + sizeof(float));
        }

        std::memcpy(&m_byteStream[m_byteIndex], &value, sizeof(float));
        m_byteIndex += sizeof(float);
    }

    void ByteStreamWriter::Write(const std::string& key, uint64 value)
    {
        WriteInt(value);
    }

    void ByteStreamWriter::Write(const std::string& key, sint64 value)
    {
        WriteInt(value);
    }
}