#include "Config.h"
#include <Util/FileUtil.h>
#include <sstream>
#include <fstream>
#include <filesystem>

namespace CitrusCore
{
	bool Config::LoadConfig(std::string file)
	{
		// Clear the current properties.
		m_properties.clear();

		// Open file 
		std::ifstream fStream;
		std::string filePath = CitrusCore::PlatformTools::GetPlatformDataPath(file);

		fStream.open(filePath.c_str(), std::ifstream::in);

		if (fStream.fail()) {
			Logger::Log(Logger::ERROR, "Unable to load configuration for: '%s'", filePath.c_str());
			fStream.close();
			return false;
		}

		// The current line being parsed 
		std::string line;
		bool bGroupProperty = false;
		std::string groupIdentifier;

		while (std::getline(fStream, line))
		{
			if (line.length() <= 0)
			{
				bGroupProperty = false;
				continue;
			}

			if (line[0] == '[')
			{
				size_t lineEnd = line.find_first_of(']');

				if (lineEnd > 0)
				{
					// Start of group 
					bGroupProperty = true;
					groupIdentifier = line.substr(1, lineEnd - 1);
					Logger::Log(Logger::VERBOSE, "Group found [%s]", groupIdentifier.c_str());
				}
			}
			else
			{
				// Try and find the first occurance of an '=' else continue 
				size_t pos = line.find_first_of('=');

				// If '=' could not be found 
				if (pos == std::string::npos)
				{
					bGroupProperty = false;
					continue;
				}

				//else seperate the string + create a property. 
				std::string key = line.substr(0, pos);
				size_t endOfLine = line.find("\n");
				std::string value = line.substr(pos + 1, endOfLine);
				std::remove_if(value.begin(), value.end(), isspace);

				if (!value.empty())
				{
					if (value.back() == '\r')
					{
						Logger::Log(Logger::VERBOSE, "CR found in [%s]", value.c_str());
						value.pop_back();
						value.push_back('\0');
					}

					if (value.back() == '\n')
					{
						Logger::Log(Logger::VERBOSE, "NL found in [%s]", value.c_str());
						value.pop_back();
						value.push_back('\0');
					}
				}

				Property<std::string, std::string> property(key, value);

				if (bGroupProperty)
				{
					m_propertyGroup[groupIdentifier].push_back(property);
					Logger::Log(Logger::VERBOSE, "Adding Property to group [%s] property:[%s] value: [%s]", groupIdentifier.c_str(), property.key.c_str(), value.c_str());
				}
				else
				{
					m_properties.push_back(property);
					Logger::Log(Logger::VERBOSE, "Adding Property:[%s] value: [%s]", property.key.c_str(), value.c_str());
				}
			}
		}

		fStream.close();
		return true;
	}

	bool Config::Save(std::string File)
	{
		std::ofstream fStream;

		std::filesystem::path fpath(File);

		if (!std::filesystem::exists(fpath.parent_path()))
		{
			std::filesystem::create_directories(fpath.parent_path());
		}

		fStream.open(File.c_str(), std::fstream::out);

		if (fStream.fail()) {
			CitrusCore::Log(Logger::ERROR, "Unable to write configuration: '%s'", File.c_str());
			fStream.close();
			return false;
		}

		for (auto group : m_propertyGroup)
		{
			std::string groupstr = ((std::string)"[" + group.first + "]\n");

			fStream.write(groupstr.c_str(), groupstr.length());
			for (auto property : group.second)
			{
				std::string str = (std::string)(property.key + "=" + property.value + "\n");
				fStream.write(str.c_str(), str.length());
			}
			fStream.write("\n", 1);
		}

		for (auto property : m_properties)
		{
			std::string str = (std::string)(property.key + "=" + property.value + "\n");
			fStream.write(str.c_str(), str.length());
		}

		fStream.close();
		return true;
	}

	std::string Config::GetString(std::string PropertyName)
	{
		for (const auto& iter : m_properties)
		{
			if (iter.key == PropertyName)
			{
				return iter.value;
			}
		}

		return "";
	}

	std::string Config::GetString(std::string PropertyName, std::string PropertyGroup)
	{
		if (m_propertyGroup.find(PropertyGroup) != m_propertyGroup.end())
		{
			for (const auto& iter : m_propertyGroup[PropertyGroup])
			{
				if (iter.key == PropertyName)
				{
					return iter.value;
				}
			}
		}

		return "";
	}

	void Config::SetString(std::string property, std::string value)
	{
		m_properties.push_back(Property<std::string, std::string>(property, value));
	}

	void Config::SetGroupProperty(std::string group, std::string property, std::string value)
	{
		m_propertyGroup[group].push_back(Property<std::string, std::string>(property, value));
	}

	std::vector<std::string> Config::GetProperties(std::string PropertyName)
	{
		std::vector<std::string> _return;

		std::vector<Property<std::string, std::string>>::iterator iter;

		for (iter = m_properties.begin(); iter != m_properties.end(); ++iter) {
			if (iter->key == PropertyName) {
				_return.push_back(iter->value);
			}
		}

		return _return;
	}

	std::vector<Property<std::string, std::string>> Config::GetGroupProperties(std::string PropertyGroup)
	{
		if (m_propertyGroup.find(PropertyGroup) != m_propertyGroup.end())
		{
			return m_propertyGroup[PropertyGroup];
		}

		return std::vector<Property<std::string, std::string>>();
	}

	std::vector<std::string> Config::GetGroupPropertiesValues(std::string PropertyGroup)
	{
		std::vector<std::string> values;

		if (m_propertyGroup.find(PropertyGroup) != m_propertyGroup.end())
		{
			for (const auto& property : m_propertyGroup[PropertyGroup])
			{
				values.push_back(property.value);
			}
		}

		return values;
	}

	float Config::GetFloat(std::string PropertyName, float defaultValue)
	{
		std::string property = GetString(PropertyName);

		if (property == "")
		{
			return defaultValue;
		}

		return (float)std::stod(property);
	}

	sint32 Config::GetInt(std::string PropertyName, int defaultValue)
	{
		std::vector<Property<std::string, std::string>>::iterator iter;
		std::string _val;

		bool bFound = false;

		for (iter = m_properties.begin(); iter != m_properties.end(); ++iter) {
			if (iter->key == PropertyName) {
				_val = iter->value;
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			return defaultValue;
		}

		return std::stoi(_val);
	}

	std::vector<double> Config::GetVectorD(std::string PropertyName)
	{
		// Parses a comma sseparated list of floats
		std::string property = GetString(PropertyName);
		std::size_t end = std::string::npos;
		std::size_t start = 0;

		std::vector<double> values;

		while ((end = property.find(',', start)) != std::string::npos)
		{
			values.push_back(std::stod(property.substr(start, end - start)));
			start = end + 1;
		}

		return values;
	}

	void Config::SetVectorD(std::string propertyName, std::vector<double> values)
	{
		std::string valuestr = "";

		for (double value : values)
		{
			valuestr += std::to_string(value) + ",";
		}

		SetString(propertyName, valuestr);
	}
}
