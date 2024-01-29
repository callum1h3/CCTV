#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H 

namespace Engine
{
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::vec2   Size;      // Size of glyph
		glm::vec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

	struct Font
	{
	public:
		std::unordered_map<GLchar, Character> characters;
		float height;

		float GetTextWidth(float scale, std::string text);
	};

	class Renderer
	{
	public:
		static void Init();
		static void Render();
		static void EndRender();
		static void Dispose();

		static glm::vec2 ScreenMin();
		static glm::vec2 ScreenMax();
		static glm::vec2 ScreenSize();

		static glm::vec2 viewPosition, viewLerped;
		static float zoom, currentzoom;

		static unsigned int background_size;

		static Font LoadFont(std::string fontname);
		static float GetTextWidth(std::string font, float scale, std::string text);
		static void DrawTextA(glm::vec2 position, glm::vec4 color, float scale, Font font, std::string text);
		static void DrawTextA(glm::vec2 position, glm::vec4 color, float scale, std::string font, std::string text);
		static void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static bool IsMouseWithinBounds(glm::vec2 start, glm::vec2 end);
		static glm::vec2 PixelToScreen(glm::vec2 input);
	private:
		static unsigned int FBO;
		static unsigned int RBO;
		static unsigned int FBO_Texture;
		static unsigned int quadVAO, quadVBO;
		static unsigned int screenQuadVAO, screenQuadVBO;
		static unsigned int fontVAO, fontVBO;
		static glm::ivec2 lastWindowSize;
		static Shader* UIShader;
		static Shader* TextShader;
		static Shader* screenShader;
		static Shader* backgroundShader;
		static FT_Library ft;

		static unsigned int UI_UBO;

		static std::unordered_map<std::string, Font> fonts;

		static void OnWindowResize();
		static void DrawUI();
		static void ScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void CreateFBO();
	};
}