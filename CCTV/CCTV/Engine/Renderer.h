#pragma once

#include "glm/glm.hpp"
#include "Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void Render();
		static void Dispose();
	private:
		static unsigned int FBO;
		static unsigned int FBO_Texture;
		static glm::ivec2 lastWindowSize;
		static Shader* UIShader;

		static void OnWindowResize();
		static void DrawUI();
	};
}