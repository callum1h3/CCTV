#include "OSHandler.h"

#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Engine;

std::string OSHandler::PATH;

void OSHandler::Initialize()
{
#ifdef _WIN32
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	PATH = std::string(buffer).substr(0, pos);
#endif
}

std::string OSHandler::GetPath()
{
	return PATH;
}

json OSHandler::LoadJson(const char* path)
{
	std::ifstream f(path);
	std::string jp = "";

	json j = json::parse("{}");

	if (f.good())
	{
		jp = std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
		try
		{
			j = json::parse(jp);

		}
		catch (json::parse_error& ex) {}
	}

	return j;
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char*)lpData;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}