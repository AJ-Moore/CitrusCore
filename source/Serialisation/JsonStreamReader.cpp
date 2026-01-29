#include <Util/Logger.h>
#include <Serialisation/JsonStreamReader.h>
#include <nlohmann/json.hpp>

namespace CitrusCore {
    JsonStreamReader::JsonStreamReader(std::shared_ptr<nlohmann::json> json)
    {
        m_json = json;
    }

    bool JsonStreamReader::BeginReadObject(const std::string& key)
    {
        nlohmann::json::iterator child = GetActiveObject().find(key);

        if (child == GetActiveObject().end() || !child->is_object())
        {
            Logger::Log(Logger::WARN, "Unable to find object [%s]", key.c_str());
            return false;
        }

        m_readStack.push(&(*child));
        return true;
    }

    void JsonStreamReader::EndReadObject()
    {
        m_readStack.pop();
    }

    bool JsonStreamReader::BeginReadArray(const std::string& key)
    {
        nlohmann::json::iterator arr = GetActiveObject().find(key);

        if (arr == GetActiveObject().end() || !arr->is_array())
        {
            Logger::Log(Logger::WARN, "Unable to find array [%s]", key.c_str());
            return false;
        }

        m_arrayStack.push(&(*arr));
        return true;
    }

    void JsonStreamReader::EndReadArray()
    {
        m_arrayStack.pop();
    }

    float JsonStreamReader::ReadFloat(const std::string& key, float defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    bool JsonStreamReader::ReadBool(const std::string& key, bool defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    std::string JsonStreamReader::ReadString(const std::string& key, const std::string& defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }
    
    nlohmann::json& JsonStreamReader::GetActiveObject()
    {
        if (m_readStack.empty())
        {
            return *m_json;
        }

        return *m_readStack.top();
    }
}