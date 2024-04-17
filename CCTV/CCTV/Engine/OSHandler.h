#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <shlobj.h>

using json = nlohmann::json;

namespace Engine
{
	class __declspec(dllexport) OSHandler
	{
	public:
		static void Initialize();
		static std::string GetPath();
		static json LoadJson(const char* path);
	private:
		static std::string PATH;
	};
}