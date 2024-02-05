#include "Application.h"

#include "Engine/Renderer.h"
#include "Engine/OSHandler.h"
#include "Engine/Shader.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "Engine/Resources.h"

#include "CCTVEngine/CCTVManager.h"

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
	Engine::Input::Initialize();

	CCTVManager::OnInitialize();

	glEnable(GL_MULTISAMPLE);
	
	while (!window.ShouldClose())
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		window.PollEvents();

		Engine::Time::Update();

		Engine::Renderer::Render();

		Engine::Renderer::EndRender();

		glfwSwapBuffers(window.Get());
	}

	Engine::Resources::Clear();
	Engine::Renderer::Dispose();
	Engine::ShaderManager::OnExit();
	window.Dispose();
}

Engine::Window* Application::GetWindow()
{
	return &window;
}