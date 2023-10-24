#pragma once

#include "Window.h"

namespace Engine
{
	class Input
	{
	public:
		static void Initialize();

		static int GetKey(int key);
		static int GetMouse(int key);
		static void GetCursor(double* x, double* y);
		static void SetInputMode(int state);
		static void GetMouseFPSStyle(double* x, double* y);
	private:
		static Window* window;
		static bool isFirstFPSStyle;
	};
}