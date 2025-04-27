#pragma once 

#include <CCCommon.h>
#include <string>
#include <algorithm>

namespace CitrusCore
{
    class CITRUS_CORE_API StringUtil
    {
    public:
        static constexpr std::string ToLower(const std::string& str) {
            std::string lowerStr = str;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return lowerStr;
        }
    };
}
