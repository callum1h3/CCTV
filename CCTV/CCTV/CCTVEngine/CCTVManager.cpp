#include "CCTVManager.h"
#include "../Engine/Renderer.h"
#include "../Application.h"
#include "../Engine/Window.h"
#include "../Engine/Resources.h"
#include "../Engine/Input.h"
#include "../Engine/Message.h"
#include <iostream>
#include "../Engine/Time.h"

// Mode Switching Icons
Engine::Texture2D* CCTVManager::editModeIcon;
Engine::Texture2D* CCTVManager::cameraModeIcon;

bool CCTVManager::isEditting = false;
float CCTVManager::editButtonTime;

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

	if (Engine::Renderer::DrawButton(glm::vec2(windowSize.x - 64, 0), glm::vec2(64, 64), glm::vec4(1, 1, 1, 1)))
		EditButtonToggle();

	float x_position = windowSize.x - (64 * Engine::Renderer::constructionBarLerp);

	if (Engine::Renderer::DrawButton(glm::vec2(x_position, 68), glm::vec2(64, 64), glm::vec4(1, 1, 1, 1)))
		EditButtonToggle();
}

void CCTVManager::EditButtonToggle()
{
	if (editButtonTime > Engine::Time::FixedTime())
		return;
	editButtonTime = Engine::Time::FixedTime() + 2.0f;

	isEditting = !isEditting;
	std::cout << "Toggled Edit Mode" << std::endl;
	

	if (isEditting)
	{
		Engine::Message::FastMessage("Switch to Edit Mode");
		Engine::Renderer::constructionBar = 1.0f;
	}
	else
	{
		Engine::Message::FastMessage("Switch to Camera Mode");
		Engine::Renderer::constructionBar = -1.0f;
	}
	
}