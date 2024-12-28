#pragma once
#include "json.hpp"
#include <string>

class JSONHelper
{
public:
	static nlohmann::json GetObject(nlohmann::json entry, const std::string& object)
	{
		return entry[object.c_str()];
	}

	template<typename T>
	static T ReadAs(nlohmann::json object, const std::string& key)
	{
		return object.at(key).get<T>();
	}

	static glm::vec3 ReadVec3(nlohmann::json object, const std::string& key)
	{
		if (object.at(key).is_array())
		{
			return { object[key][0], object[key][1], object[key][2] };
		}

		return { 0, 0, 0 };
	}

	static glm::vec4 ReadVec4(nlohmann::json object, const std::string& key)
	{
		if (object.at(key).is_array())
		{
			return { object[key][0], object[key][1], object[key][2], object[key][3]};
		}

		return { 0, 0, 0, 0 };
	}

};

