#include "CCTVManager.h"
#include "../Engine/Renderer.h"
#include "../Application.h"
#include "../Engine/Window.h"
#include "../Engine/Resources.h"

// Mode Switching Icons
Engine::Texture2D* CCTVManager::editModeIcon;
Engine::Texture2D* CCTVManager::cameraModeIcon;

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
	Engine::Renderer::DrawRect(glm::vec2(windowSize.x - 32, 0), glm::vec2(32, 32), glm::vec4(1, 1, 1, 1));

	//Engine::Renderer::IsMouseWithinBounds()
}

void CCTVManager::EditButtonToggle()
{

}