#pragma once 

#include "CCCommon.h"
namespace CitrusCore 
{
    class CITRUS_CORE_API ITimestampProvider {
        public:
            virtual ~ITimestampProvider() = default;
            virtual double GetTimestamp() = 0;
    };
}
