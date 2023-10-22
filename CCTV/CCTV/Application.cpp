#include "Application.h"

#include "Engine/Renderer.h"
#include "Engine/OSHandler.h"
#include "Engine/Shader.h"

Engine::Window Application::window;

void Application::Run()
{
	if (!glfwInit())
		return;

	window.Init();
	window.SetWindowName("CCTV System");

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return;

	Engine::OSHandler::Initialize();
	Engine::ShaderManager::Initialize();
	Engine::Renderer::Init();

	while (!window.ShouldClose())
	{
		window.PollEvents();

		Engine::Renderer::Render();

		glfwSwapBuffers(window.Get());
	}

	Engine::Renderer::Dispose();
	Engine::ShaderManager::OnExit();
	window.Dispose();
}

Engine::Window* Application::GetWindow()
{
	return &window;
}