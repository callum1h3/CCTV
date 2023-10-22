#pragma once

#ifndef WINDOW_INCLUDE
#define WINDOW_INCLUDE

#include "OpenGL.h"
#include "glm/glm.hpp"

namespace Engine
{
	// Initializes a simple GLFW window.
	class Window
	{
	public:
		bool Init();
		bool Init(Window* window);

		void Dispose();

		GLFWwindow* Get();
		bool ShouldClose();
		void MakeCurrentContext();
		void PollEvents();
		void ClearNSwap();

		void SetSize(glm::ivec2 size);
		glm::ivec2 GetSize();

		void SetWindowName(const char* name);
	private:
		GLFWwindow* _window;
	};
}

#endif