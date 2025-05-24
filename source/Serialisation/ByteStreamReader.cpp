
#include "Util/Logger.h"
#include <Serialisation/ByteStreamReader.h>

namespace CitrusCore {
    ByteStreamReader::ByteStreamReader(uint8* src, size_t size)
    {
        m_byteStream = src; 
        m_size = size; 
        m_readIndex = 0;
    }

    float ByteStreamReader::ReadFloat()
    {
        if (m_readIndex + sizeof(float) >= m_size)
        {
            Logger::Log(Logger::ERROR, "Unable to read bytes from bytes stream, reached end of stream.");
        }

        float result;
        std::memcpy(&result, &m_byteStream[m_readIndex], sizeof(float));
        m_readIndex += sizeof(float);
        return result;
    }
}