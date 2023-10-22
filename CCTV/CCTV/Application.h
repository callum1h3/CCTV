#pragma once

#include "Engine/Window.h"

class Application
{
public:
	static void Run();

	static Engine::Window* GetWindow();
private:
	static Engine::Window window;
};