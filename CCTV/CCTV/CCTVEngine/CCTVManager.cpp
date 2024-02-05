#include "CCTVManager.h"
#include "../Engine/Renderer.h"
#include "../Application.h"
#include "../Engine/Window.h"
#include "../Engine/Resources.h"
#include "../Engine/Input.h"
#include <iostream>

// Mode Switching Icons
Engine::Texture2D* CCTVManager::editModeIcon;
Engine::Texture2D* CCTVManager::cameraModeIcon;

bool CCTVManager::isEditting = false;

void CCTVManager::OnInitialize()
{
	editModeIcon = Engine::Resources::Load<Engine::Texture2D>("icons/edit.png");
	cameraModeIcon = Engine::Resources::Load<Engine::Texture2D>("icons/edit.png");
}

void CCTVManager::OnMoveableRender()
{




	
}

void CCTVManager::OnStaticRender()
{
	Engine::Window* window = Application::GetWindow();
	glm::ivec2 windowSize = window->GetSize();

	glm::vec2 editButtonPosition = glm::vec2(windowSize.x - 32, 0);
	glm::vec2 editButtonPositionMax = editButtonPosition + glm::vec2(32, 32);
	Engine::Renderer::DrawRect(glm::vec2(windowSize.x - 32, 0), glm::vec2(32, 32), glm::vec4(1, 1, 1, 1));

	if (Engine::Renderer::IsMouseWithinBounds(editButtonPosition, editButtonPositionMax) && Engine::Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT))
		EditButtonToggle();
}

void CCTVManager::EditButtonToggle()
{
	isEditting = !isEditting;
	std::cout << "Toggled Edit Mode" << std::endl;

	if (isEditting)
	{
		Engine::Renderer::constructionBar = 1.0f;
	}
	else
	{
		Engine::Renderer::constructionBar = -1.0f;
	}
	
}