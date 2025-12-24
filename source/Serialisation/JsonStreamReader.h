#pragma once
#include <CCCommon.h>
#include <Serialisation/Stream.h>
#include <nlohmann/json.hpp>
#include <stack>

namespace CitrusCore 
{
    class CITRUS_CORE_API JsonStreamReader : public StreamReader
    {
    public:
        JsonStreamReader() = delete; 
        JsonStreamReader(std::shared_ptr<nlohmann::json> json);
        virtual ~JsonStreamReader(){}

        virtual bool BeginReadObject(const std::string& key) override;
        virtual void EndReadObject() override;

        virtual bool BeginReadArray(const std::string& key) override;
        virtual void EndReadArray() override;

        virtual float ReadFloat(const std::string& key, float defaultValue) override;
        virtual bool ReadBool(const std::string& key, bool defaultValue) override;
        virtual std::string ReadString(const std::string& key, const std::string& defaultValue) override;
    private:
        nlohmann::json& GetActiveObject();

        std::shared_ptr<nlohmann::json> m_json;
        std::stack<nlohmann::json*> m_readStack;
        std::stack<nlohmann::json*> m_arrayStack;
    };
}