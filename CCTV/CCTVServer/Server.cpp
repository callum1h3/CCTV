#include "Server.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void Server::Run()
{
	while (true)
	{
		ThreadSleep(1000 / SERVER_TICK_SPEED);

		std::cout << "hi";
	}
}

void Server::ThreadSleep(int milliseconds)
{
	#ifdef _WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds * 1000);
	#endif
}