/*
* Created by blackdragonx61
*/
#pragma once
#include <iostream>
#include <filesystem>
#include "ProtoTool.h"
#include <sstream>
// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace ReadBitch
{
	inline auto Split(const std::string& s, char delimiter)
	{
		ProtoTool::STRING_VEC tokens;

		std::string token;
		std::istringstream tokenStream(s);

		while (std::getline(tokenStream, token, delimiter))
			tokens.emplace_back(token);

		return tokens;
	}

	template <typename T> inline auto GetValue(const json& j, const std::string& BaseName, T& ret)
	{
		try
		{
			ret = j.at(BaseName);
		}
		catch (json::exception& e)
		{
			printf("[%s] Error: %s\n", BaseName.c_str(), e.what());
			return false;
		}

		return true;
	}

	/// Currently not using...
	template <typename T> inline auto GetColumnValue(const json& j, const std::string& BaseName, const std::string& SubName, T& ret)
	{
		try
		{
			const auto& cm = j.at(BaseName);
			GetValue(cm, SubName, ret);
		}
		catch (json::exception& e)
		{
			printf("[%s] Error: %s\n", BaseName.c_str(), e.what());
			return false;
		}

		return true;
	}
	
	inline auto GetValue(const json& j, const std::string& BaseName, ProtoTool::STRING_VEC& vec)
	{
		try
		{
			const auto& section = j.at(BaseName);

			for (const auto& v : section.items())
			{
				const auto& val = v.value();

				if (val.is_boolean() == false)
				{
					printf("[%s::%s] must be boolean.\n", BaseName.c_str(), v.key().c_str());
					return false;
				}

				if (val == true)
					vec.emplace_back(v.key());
			}
		}
		catch (json::exception& e)
		{
			printf("[%s] Error: %s\n", BaseName.c_str(), e.what());
			return false;
		}

		return true;
	}

	template<std::size_t N> inline auto GetValue(const json& j, const std::string& BaseName, const ProtoTool::USKIPMAP& Map, std::bitset<N>& bSet)
	{
		try
		{
			const auto& ItemSkipColumn = j.at(BaseName);
			for (const auto& [key, val] : Map)
			{
				const auto& bSetting = ItemSkipColumn.at(key);
				if (bSetting.is_boolean())
				{
					bSet.set(val, bSetting);
				}
				else
				{
					printf("[%s::%s] must be boolean.\n", BaseName.c_str(), key.c_str());
					return false;
				}
			}
		}
		catch (json::exception& e)
		{
			printf("[%s] Error: %s\n", BaseName.c_str(), e.what());
			return false;
		}

		return true;
	}
};