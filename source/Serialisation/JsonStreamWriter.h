#pragma once
#include <CCCommon.h>
#include <Serialisation/Stream.h>
#include <nlohmann/json.hpp>
#include <stack>


namespace CitrusCore 
{
    using json = nlohmann::json;

    class CITRUS_CORE_API JsonStreamWriter : public StreamWriter
    {
    public:
        virtual void BeginObject(const std::string& key);
        virtual void EndObject(); 

        virtual void BeginArray(const std::string& key);
        virtual void EndArray();

        virtual void Write(const std::string& key, std::string value);
        virtual void Write(const std::string& key, bool value);
        virtual void Write(const std::string& key, float value);
        virtual void Write(const std::string& key, uint64 value);
        virtual void Write(const std::string& key, sint64 value);
        virtual void Write(const std::string& key, json json);

        const json& GetJson() const { return m_json; }

    private:
        nlohmann::json& GetActiveObject();
        std::stack<nlohmann::json*> m_writeStack;
        json m_json;
    };
}