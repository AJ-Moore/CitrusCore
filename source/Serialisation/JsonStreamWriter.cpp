#include <Serialisation/JsonStreamWriter.h>
#include <nlohmann/json.hpp>

namespace CitrusCore
{
    void JsonStreamWriter::BeginObject(const std::string& key)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(json::object());
            m_writeStack.push(&obj.back());
        }
        else {
            obj[key] = json::object();
            m_writeStack.push(&obj[key]);
        }
    }

    void JsonStreamWriter::EndObject()
    {
        m_writeStack.pop();
    }

    void JsonStreamWriter::BeginArray(const std::string& key)
    {
        json &obj = GetActiveObject();
        obj[key] = json::array_t();
        m_writeStack.push(&obj[key]);
    }

    void JsonStreamWriter::EndArray()
    {
        m_writeStack.pop();
    }

    void JsonStreamWriter::Write(const std::string& key, std::string value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    void JsonStreamWriter::Write(const std::string& key, bool value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    void JsonStreamWriter::Write(const std::string& key, float value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    void JsonStreamWriter::Write(const std::string& key, uint64 value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    void JsonStreamWriter::Write(const std::string& key, sint64 value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    void JsonStreamWriter::Write(const std::string& key, json value)
    {
        json &obj = GetActiveObject();
        
        if (obj.is_array())
        {
            obj.push_back(value);
        }
        else {
            obj[key] = value;
        }
    }

    nlohmann::json& JsonStreamWriter::GetActiveObject()
    {
        if (m_writeStack.empty())
        {
            return m_json;
        }

        return *m_writeStack.top();
    }
}