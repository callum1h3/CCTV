#include "Renderer.h"
#include "OpenGL.h"
#include <iostream>
#include "../Application.h"

using namespace Engine;

unsigned int Renderer::FBO;
unsigned int Renderer::FBO_Texture;
glm::ivec2 Renderer::lastWindowSize;
Shader* Renderer::UIShader;

void Renderer::Init()
{
	lastWindowSize = glm::ivec2(0,0);
	UIShader = ShaderManager::GetShader("ui");

	// Creating Frame Buffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Creating texture for the frame buffer.
	glGenTextures(1, &FBO_Texture);
	glBindTexture(GL_TEXTURE_2D, FBO_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_Texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);


	// Checking if frame buffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to create framebuffer\n";
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	DrawUI();
}

void Renderer::Dispose()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &FBO_Texture);
}

void Renderer::DrawUI()
{

}

void Renderer::OnWindowResize()
{
	glBindTexture(GL_TEXTURE_2D, FBO_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lastWindowSize.x, lastWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}