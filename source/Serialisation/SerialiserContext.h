#include <CCCommon.h>

namespace CitrusCore 
{
    class CITRUS_CORE_API Stream 
    {
        
    };

    class CITRUS_CORE_API StreamReader
    {

    };

    class CITRUS_CORE_API StreamWriter : public Stream
    {
    public:
        virtual void WriteString(std::string value);
        virtual void WriteBool(bool value);
        virtual void WriteFloat(float value);
    };
}