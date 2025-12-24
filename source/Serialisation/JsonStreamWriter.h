
#include <CCCommon.h>
#include <Serialisation/Stream.h>
#include <nlohmann/json.hpp>


namespace CitrusCore 
{
    using json = nlohmann::json;

    class CITRUS_CORE_API JsonStreamWriter : public StreamWriter
    {
    public:
        virtual void WriteString(std::string value) = 0;
        virtual void WriteBool(bool value) = 0;
        virtual void WriteFloat(float value) = 0;
        virtual void WriteUnsigned(uint64 value) = 0;
        virtual void WriteSigned(sint64 value) = 0;

    private:
        json m_json;
    };
}