#pragma once

#include <CCCommon.h>
#include <Types.h>
#include <cstddef>
#include <cstring>

namespace CitrusCore 
{
    class CITRUS_CORE_API ByteStreamReader
    {
    public:
        ByteStreamReader() = delete;
        ByteStreamReader(uint8* src, size_t size);

        float ReadFloat();
    private:
        uint8* m_byteStream;
        int m_size = 0;
        int m_readIndex = 0;
    };
}