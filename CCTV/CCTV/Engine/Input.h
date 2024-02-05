#pragma once

#include "Window.h"
#include <unordered_map>
#include <vector>

namespace Engine
{
	struct Key
	{
	public:
		bool isUp = true;
		bool pressedThisFrame = false;

	};

	class Input
	{
	public:
		static void Initialize();
		static void LateUpdate();

		static bool GetKey(int key);
		static bool IsKeyDown(int key);
		static bool IsKeyUp(int key);

		static bool GetMouse(int key);
		static bool IsMouseDown(int key);
		static bool IsMouseUp(int key);

		static void GetCursor(double* x, double* y);
		static void SetInputMode(int state);
		static void GetMouseFPSStyle(double* x, double* y);
	private:
		static Window* window;
		static bool isFirstFPSStyle;
		static std::vector<Key*> keys_vector;
		static std::unordered_map<int, Key*> keys;

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);



	};
}