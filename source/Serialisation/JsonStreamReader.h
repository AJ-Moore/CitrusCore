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

        virtual bool ReadArrayIndex(uint32_t index) override;
        virtual bool ReadNextArrayIndex() override;

        virtual uint32_t ReadUnsigned(const std::string& key, uint32_t defaultValue = 0) override;
        virtual int32_t ReadSigned(const std::string& key, int32_t defaultValue = 0) override;
        virtual float ReadFloat(const std::string& key, float defaultValue) override;
        virtual bool ReadBool(const std::string& key, bool defaultValue) override;
        virtual std::string ReadString(const std::string& key, const std::string& defaultValue) override;

        template <class T> 
        T Read(const std::string& key)
        {
            GetActiveObject().get<T>(key);
        }
    private:
        nlohmann::json& GetActiveObject();

        std::shared_ptr<nlohmann::json> m_json;
        std::stack<nlohmann::json*> m_readStack;
        int32_t m_arrayIndex = 0;
    };
}