#pragma once

#include <CCCommon.h>

namespace CitrusCore 
{
    class CITRUS_CORE_API Stream 
    {
        
    };

    class CITRUS_CORE_API StreamReader : public Stream
    {
    public:
        virtual bool BeginReadObject(const std::string& key) = 0;
        virtual void EndReadObject() = 0;

        virtual bool BeginReadArray(const std::string& key) = 0;
        virtual void EndReadArray() = 0;

        virtual float ReadFloat(const std::string& key, float defaultValue) = 0;
        virtual bool ReadBool(const std::string& key, bool defaultValue) = 0;
        virtual std::string ReadString(const std::string& key, const std::string& defaultValue) = 0;
    };

    class CITRUS_CORE_API StreamWriter : public Stream
    {
    public:
        virtual void BeginObject(const std::string& key) = 0;
        virtual void EndObject() = 0; 

        virtual void BeginArray(const std::string& key) = 0;
        virtual void EndArray() = 0;

        virtual void Write(const std::string& key, std::string value) = 0;
        virtual void Write(const std::string& key, bool value) = 0;
        virtual void Write(const std::string& key, float value) = 0;
        virtual void Write(const std::string& key, uint64 value) = 0;
        virtual void Write(const std::string& key, sint64 value) = 0;
    };
}