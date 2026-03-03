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

        m_readStack.push(&child.value());
        return true;
    }

    void JsonStreamReader::EndReadObject()
    {
        m_readStack.pop();
    }

    bool JsonStreamReader::BeginReadArray(const std::string& key)
    {
        m_arrayIndex = 0;
        nlohmann::json::iterator arr = GetActiveObject().find(key);

        if (arr == GetActiveObject().end() || !arr->is_array())
        {
            // Things will break if the object isn't what it thinks it is, only pop if (BeginReadArray()) returns true.
            Logger::Log(Logger::ERROR, "Unable to find array [%s]", key.c_str());
            return false;
        }

        m_readStack.push(&arr.value());
        // Urg we set to -1 mainly for the convience of ReadNextArrayIndex 
        m_arrayIndex = -1;
        return true;
    }

    void JsonStreamReader::EndReadArray()
    {
        m_readStack.pop();
    }

    uint32_t JsonStreamReader::ReadUnsigned(const std::string& key, uint32_t defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    int32_t JsonStreamReader::ReadSigned(const std::string& key, int32_t defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    float JsonStreamReader::ReadFloat(const std::string& key, float defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    bool JsonStreamReader::ReadBool(const std::string& key, bool defaultValue)
    {
        return GetActiveObject().value(key, defaultValue);
    }

    bool JsonStreamReader::ReadNextArrayIndex() 
    {
        return ReadArrayIndex(++m_arrayIndex);
    }

    bool JsonStreamReader::ReadArrayIndex(uint32_t index)
    {
        if (m_readStack.empty())
        {
            return false;
        }

        if (!m_readStack.top()->is_array())
        {
            // If current object top stack not array return false.
            return false;
        }

        if (index >= m_readStack.top()->size())
        {
            // Out of range
            Logger::Log(Logger::VERBOSE, "Array index out of range.");
            return false;
        }

        m_arrayIndex = index;
        return true;
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

        if (m_readStack.top()->is_array())
        {
            return m_readStack.top()->at(m_arrayIndex);
        }

        return *m_readStack.top();
    }
}