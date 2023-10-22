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
		static void MBox(const wchar_t* title, const wchar_t* desc);
		static bool ErrorCheck(const wchar_t* error);
		static json LoadJson(const char* path);
		static std::string BrowseFolder(std::string saved_path);
	private:
		static std::string PATH;
	};
}