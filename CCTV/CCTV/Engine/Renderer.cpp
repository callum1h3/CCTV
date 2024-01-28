#include "Renderer.h"
#include "OpenGL.h"

#include <iostream>
#include <cmath>
#include <cassert>

#include "../Application.h"
#include "Input.h"
#include "Time.h"
#include "OSHandler.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Message.h"
#include "MathHelper.h"

using namespace Engine;

// Buffers
unsigned int Renderer::FBO;
unsigned int Renderer::RBO;
unsigned int Renderer::FBO_Texture;
unsigned int Renderer::quadVAO, Renderer::quadVBO;
unsigned int Renderer::fontVAO, Renderer::fontVBO;
unsigned int Renderer::screenQuadVAO, Renderer::screenQuadVBO;

// Shaders
Shader* Renderer::UIShader;
Shader* Renderer::TextShader;
Shader* Renderer::screenShader;


glm::ivec2 Renderer::lastWindowSize;
FT_Library Renderer::ft;
glm::vec2 Renderer::viewPosition, Renderer::viewLerped;
float Renderer::zoom = 1, Renderer::currentzoom = 1;

unsigned int Renderer::UI_UBO;

std::unordered_map<std::string, Font> Renderer::fonts;

const float screenQuadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	0,  1.0f,  0.0f, 1.0f,
	0, 0,  0.0f, 0.0f,
	 1.0f, 0,  1.0f, 0.0f,

	0,  1.0f,  0.0f, 1.0f,
	 1.0f, 0,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

void Renderer::Init()
{
	viewPosition = glm::vec2(0, 0);
	lastWindowSize = glm::ivec2(0,0);

	// Getting Shaders
	UIShader = ShaderManager::GetShader("ui");
	TextShader = ShaderManager::GetShader("text");
	screenShader = ShaderManager::GetShader("screen");

	// Creating Quad Buffer
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Creating Screen Quad Buffer
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Initializing font library
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Could not init FreeType Library\n";
		return;
	}

	// Font VBO VAO
	glGenVertexArrays(1, &fontVAO);
	glGenBuffers(1, &fontVBO);
	glBindVertexArray(fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize UI Projection

	glGenBuffers(1, &UI_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UI_UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	ShaderManager::CreateGlobalStorage("ui_storage", UI_UBO, sizeof(glm::mat4), 1);

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetScrollCallback(Application::GetWindow()->Get(), &ScrollWheelCallback);

	Message::FastMessage("SUCCESSFULLY INITIALIZED SYSTEMS!");
	Message::FastMessage("LOL");
	Message::FastMessage("EPICER");
}

void Renderer::Render()
{
	Window* window = Application::GetWindow();

	glm::ivec2 newSize = window->GetSize();
	if (newSize != lastWindowSize)
	{
		lastWindowSize = newSize;
		OnWindowResize();
	}

	int right_mouse = Input::GetMouse(GLFW_MOUSE_BUTTON_RIGHT);
	if (right_mouse == GLFW_PRESS)
	{
		Input::SetInputMode(GLFW_CURSOR_DISABLED);

		double x, y;
		Input::GetMouseFPSStyle(&x, &y);

		viewPosition.x -= (x * currentzoom);
		viewPosition.y += (y * currentzoom);


	}
	else
	{
		Input::SetInputMode(GLFW_CURSOR_NORMAL);
	}

	viewLerped.x = MathHelper::lerp(viewLerped.x, viewPosition.x, Time::DeltaTime() * 8.0f);
	viewLerped.y = MathHelper::lerp(viewLerped.y, viewPosition.y, Time::DeltaTime() * 8.0f);

	currentzoom = MathHelper::lerp(currentzoom, zoom, Time::DeltaTime() * 8.0f);

	glm::vec2 min = ScreenMin();
	glm::vec2 max = ScreenMax();

	


	// Capturing data into framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.019f, 0.050f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 moveableProjection = glm::ortho(min.x, max.x, min.y, max.y);
	glBindBuffer(GL_UNIFORM_BUFFER, UI_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &moveableProjection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	DrawUI();
	

	glm::mat4 screenProjection = glm::ortho(0.0f, (float)lastWindowSize.x, 0.0f, (float)lastWindowSize.y);
	glBindBuffer(GL_UNIFORM_BUFFER, UI_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &screenProjection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Message::Render();



}

void Renderer::DrawUI()
{
	Font font = LoadFont(OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");

	glm::vec2 min = glm::vec2(0, 0);
	glm::vec2 max = glm::vec2(min) + glm::vec2(32, 32);
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	if (IsMouseWithinBounds(min, max))
		color = glm::vec4(0, 1, 1, 1);


	Renderer::DrawRect(glm::vec2(0, 0), glm::vec2(32, 32), color);


	//DrawRect(glm::vec2(0, 0), glm::vec2(font.GetTextWidth(0.25f, "Hello"), font.height * 0.25f), glm::vec4(1.0f, 0.1f, 0.1f, 1));

	//DrawTextA(glm::vec2(0, 0), glm::vec4(1,1,1,1), 0.25f, font, "Hello");
}

void Renderer::EndRender()
{
	// Ending frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Displaying frame buffer texture onto screen.
	screenShader->Use();
	glBindVertexArray(screenQuadVAO);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBO_Texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::Dispose()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &FBO_Texture);
	glDeleteRenderbuffers(1, &RBO);
}

void Renderer::CreateFBO()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &FBO_Texture);
	glDeleteRenderbuffers(1, &RBO);

	// Creating Frame Buffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Creating texture for the frame buffer. we are only doing forward renderering so we only need a rgb texture
	glGenTextures(1, &FBO_Texture);
	glBindTexture(GL_TEXTURE_2D, FBO_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lastWindowSize.x, lastWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_Texture, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lastWindowSize.x, lastWindowSize.y); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

	// Checking if frame buffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to create framebuffer\n";
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::OnWindowResize()
{
	glViewport(0, 0, lastWindowSize.x, lastWindowSize.y);
	
	CreateFBO();
}

glm::vec2 Renderer::PixelToScreen(glm::vec2 input)
{
	input.x = input.x / lastWindowSize.x;
	input.y = input.y / lastWindowSize.y;

	return input;
}

void Renderer::ScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << xoffset << " " << yoffset << "\n";

	zoom = glm::clamp(zoom + ((float)yoffset * 0.05f), 0.1f, 4.0f);
}

Font Renderer::LoadFont(std::string fontname)
{
	if (fonts.count(fontname))
		return fonts[fontname];

	FT_Face face;
	if (FT_New_Face(ft, fontname.c_str(), 0, &face)) {
		std::cout << "Failed to load font" << std::endl;
		return Font();
	}
	else {
		Font font = Font();
		
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 512);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		float highest = 0;

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// now store character for later use
			Character character = {
				texture,
				glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			font.characters.insert(std::pair<char, Character>(c, character));

			if (face->glyph->bitmap_top > highest)
				highest = face->glyph->bitmap_top;

			glGenerateMipmap(texture);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		font.height = highest;
		fonts.insert(std::pair<std::string, Font>(fontname, font));

		return font;
	}
}

void Renderer::DrawTextA(glm::vec2 position, glm::vec4 color, float scale, Font font, std::string text)
{
	TextShader->Use();
	TextShader->UniformSetVec4(2, color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontVAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font.characters[*c];

		float xpos = position.x + ch.Bearing.x * scale;
		float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 Renderer::ScreenMin()
{
	return viewLerped;
}

glm::vec2 Renderer::ScreenMax()
{
	return viewLerped + ((glm::vec2)lastWindowSize * currentzoom);
}

glm::vec2 Renderer::ScreenSize()
{
	return lastWindowSize;
}

bool Renderer::IsMouseWithinBounds(glm::vec2 start, glm::vec2 end)
{
	double x, y;
	Input::GetCursor(&x, &y);

	x *= zoom;
	y *= zoom;

	glm::vec2 min = ScreenMin();
	glm::vec2 max = ScreenMax();
	x += min.x;
	y = max.y - y;

	if (x > start.x && y > start.y)
		if (x < end.x && y < end.y)
			return true;

	return false;

}

void Renderer::DrawTextA(glm::vec2 position, glm::vec4 color, float scale, std::string font, std::string text)
{
	Font f = LoadFont(font);

	DrawTextA(position, color, scale, f, text);
}

void Renderer::DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
	UIShader->UniformSetVec2(2, position);
	UIShader->UniformSetVec2(3, size);
	UIShader->UniformSetVec4(4, color);
	UIShader->Use();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

float Renderer::GetTextWidth(std::string font, float scale, std::string text)
{
	Font f = LoadFont(font);
	return f.GetTextWidth(scale, text);
}

float Font::GetTextWidth(float scale, std::string text)
{
	float width = 0;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];
		width += (ch.Advance >> 6) * scale;
	}

	return width;
}