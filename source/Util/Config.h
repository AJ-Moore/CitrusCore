#pragma once 

#include <CCCommon.h>
#include <Util/Property.h>
#include <vector>

namespace CitrusCore
{
	class CITRUS_CORE_API Config {
	public:
		/// Loads the configuration file specified
		bool LoadConfig(std::string File);

		bool Save(std::string File);

		/// Returns the first property with the given name.
		std::string GetString(std::string PropertyName);

		/// Returns the first property with the given name in the specified group.
		std::string GetString(std::string PropertyName, std::string PropertyGroup);

		/// Adds a new property to the configuration
		void SetString(std::string property, std::string value);

		/// Adds a group property 
		void SetGroupProperty(std::string group, std::string property, std::string value);

		/// Returns a vector of strings for of all the properties with the given property name.
		std::vector<std::string> GetProperties(std::string PropertyName);

		/// Returns a vextor of properties for the given group 
		std::vector<Property<std::string, std::string>> GetGroupProperties(std::string PropertyGroup);

		/// Returns all the values for a property group, required by cube maps currently
		std::vector<std::string> GetGroupPropertiesValues(std::string PropertyGroup);

		float GetFloat(std::string PropertyName, float defaultValue);

		/// Returns an integer property with the given name.
		sint32 GetInt(std::string PropertyName, int defaultValue = 0);

		/// Returns array of doubles for a property with a given name.
		std::vector<double> GetVectorD(std::string PropertyName);

		/// Writes a vector as a comma separated list of values.
		void SetVectorD(std::string propertyName, std::vector<double> values);

	private:
		/// Holds all the properties loaded by loadConfig( ... )
		std::vector<Property<std::string, std::string>> m_properties;

		/// Property group/s, key = String group identifier, value = array of groups properties.
		typedef std::unordered_map<std::string, std::vector<Property<std::string, std::string>>> PropertyGroup;
		PropertyGroup m_propertyGroup;
	};
}
