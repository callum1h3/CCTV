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

void OSHandler::MBox(const wchar_t* title, const wchar_t* desc)
{
#ifdef _WIN32
	MessageBox(NULL, (LPCWSTR)desc, (LPCWSTR)title, MB_ICONERROR | MB_OK);
#endif
}

bool OSHandler::ErrorCheck(const wchar_t* error)
{
	if (wcslen(error) > 8)
	{
		MBox(L"An error occurred!", error);
		return true;
	}
	return false;
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

std::string OSHandler::BrowseFolder(std::string saved_path)
{
	TCHAR path[MAX_PATH];

	const char* path_param = saved_path.c_str();

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = (LPCWSTR)"Browse for folder...";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}


		TCHAR* pathp = path;
		std::basic_string<TCHAR> strName = pathp;
		std::string str(strName.begin(), strName.end());
		return str;
	}

	return "";
}