#pragma once

#include <iostream>

namespace Engine
{
	class Debug
	{
	public:
		static void Log(const char* message)
		{
			std::cout << message << "\n";
		}

		static void Log(std::string message)
		{
			Log(message.c_str());
		}
	};
}