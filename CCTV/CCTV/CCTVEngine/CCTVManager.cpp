#include "CCTVManager.h"
#include "../Engine/Renderer.h"
#include "../Application.h"
#include "../Engine/Window.h"
#include "../Engine/Resources.h"
#include "../Engine/Input.h"
#include "../Engine/Message.h"
#include <iostream>
#include "../Engine/Time.h"
#include "../Engine/MathHelper.h"
#include "../Engine/OSHandler.h"

// Mode Switching Icons
Engine::Texture2D* CCTVManager::editModeIcon;
Engine::Texture2D* CCTVManager::cameraModeIcon;

bool CCTVManager::isEditting = false;
bool CCTVManager::isSettingsOpen = false;
float CCTVManager::editButtonTime;
double CCTVManager::settingsStartTime;

std::vector<SObject*> CCTVManager::sobjects;
std::vector<SObject*> CCTVManager::sObjectTypes;
std::vector<Tool*> CCTVManager::toolTypes;
std::vector<RenderOrderContainer*> CCTVManager::render_order;
std::unordered_map<int, RenderOrderContainer*> CCTVManager::render_order_map;
Tool* CCTVManager::selected_tool = nullptr;
// Tools

bool Tool::OnPlace(glm::vec2 cursor_position) { return true; }
void Tool::OnDisplay(glm::vec2 cursor_position) {}

void SObject::OnDisplay() {}
void SObject::OnSave() {}
int SObject::GetID() { return -1; }
int SObject::GetOrder() { return 0; }

void RenderOrderContainer::Render()
{
	for (SObject* obj : objs)
		obj->OnDisplay();
}

void RenderOrderContainer::Add(SObject* obj)
{
	objs.push_back(obj);
}

void RenderOrderContainer::Remove(SObject* obj)
{
	int i = Contains(obj);

	if (i != -1)
		objs.erase(objs.begin() + i);
}

int RenderOrderContainer::Contains(SObject* obj)
{
	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i] == obj)
			return i;
	}
	
	return -1;
}

void CCTVManager::OnInitialize()
{

	//editModeIcon = Engine::Resources::Load<Engine::Texture2D>("icons/edit.png");
	//cameraModeIcon = Engine::Resources::Load<Engine::Texture2D>("icons/edit.png");
}

void CCTVManager::OnMoveableRender()
{
	for (RenderOrderContainer* container : render_order)
		container->Render();
}

void CCTVManager::OnStaticRender()
{
	Engine::Window* window = Application::GetWindow();
	glm::ivec2 windowSize = window->GetSize();
	
	if (Engine::Renderer::DrawButton(glm::vec2(windowSize.x - 64, 0), glm::vec2(64, 64), glm::vec4(1, 1, 1, 1)))
		EditButtonToggle();

	float x_position = windowSize.x - (64 * Engine::Renderer::constructionBarLerp);

	int i = 1;
	for (Tool* tool : toolTypes)
	{
		if (Engine::Renderer::DrawButton(glm::vec2(x_position, 68 * i), glm::vec2(64, 64), glm::vec4(1, 1, 1, 1)))
			SelectTool(tool);
		i++;
	}
	
		

	RenderSettings();

	double x, y;
	Engine::Renderer::GetMouseRelative(x, y);

	if (selected_tool != nullptr)
	{
		glm::vec2 mouse_position = glm::vec2(x, y);
		selected_tool->OnDisplay(mouse_position);
		
		int state = Engine::Input::GetMouse(GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			if (editButtonTime > Engine::Time::FixedTime())
				return;

			if (selected_tool->OnPlace(mouse_position))
				DeleteTool();
		}
	}
}

void CCTVManager::AddSObject(SObject* obj)
{
	sobjects.push_back(obj);

	RenderOrderContainer* container = FindContainer(obj->GetOrder());
	container->Add(obj);
}

void CCTVManager::SelectTool(Tool* tool)
{
	DeleteTool();

	selected_tool = tool;
	editButtonTime = Engine::Time::FixedTime() + 1.0f;
}

void CCTVManager::DeleteTool()
{
	if (selected_tool == nullptr)
		return;

	selected_tool = nullptr;
}

void CCTVManager::RenderSettings()
{
	if (!isSettingsOpen)
		return;


	Engine::Font font = Engine::Renderer::LoadFont(Engine::OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");

	double t = Engine::Time::FixedTime();
	float t_elapsed = (float)(t - settingsStartTime);

	Engine::Window* window = Application::GetWindow();
	glm::ivec2 windowSize = window->GetSize();

	float transpancy = MathHelper::lerpClamp(0.0f, 1.0f, t_elapsed / 0.25f);

	// Draw Background
	Engine::Renderer::DrawRect(glm::vec2(0, 0), windowSize, glm::vec4(0, 0, 0, transpancy * 0.95f));

	Engine::Renderer::EnableMouse();

	if (Engine::Renderer::DrawButton(glm::vec2(0, 0), glm::vec2(64, 64), glm::vec4(1, 1, 1, transpancy)))
		SettingsToggle();


	float title_font_size = 0.1f;
	float width = font.GetTextWidth(title_font_size, "SETTINGS");
	Engine::Renderer::DrawTextA(glm::vec2((windowSize.x / 2) - (width / 2), windowSize.y - ((font.height * title_font_size) * 2)), glm::vec4(1, 1, 1, transpancy), title_font_size, font, "SETTINGS");


	
	Engine::Renderer::DisableMouse();
}

void CCTVManager::SettingsToggle()
{
	if (editButtonTime > Engine::Time::FixedTime())
		return;
	editButtonTime = Engine::Time::FixedTime() + 1.0f;
	
	if (isSettingsOpen)
	{
		isSettingsOpen = false;
		return;
	}

	settingsStartTime = Engine::Time::FixedTime();
	isSettingsOpen = true;
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

RenderOrderContainer* CCTVManager::FindContainer(int orderID)
{
	if (render_order_map.count(orderID))
		return render_order_map[orderID];
	return nullptr;
}

void CCTVManager::FindContainerSpot(RenderOrderContainer* container)
{
	if (render_order.size() < 1)
	{
		render_order.push_back(container);
		return;
	}

	int index = 0;
	for (RenderOrderContainer* i : render_order)
	{
		if (i->orderID > container->orderID)
		{
			index++;
			continue;
		}

		render_order.insert(render_order.begin() + index, container);
		return;
	}

	render_order.push_back(container);
}

void CCTVManager::__AddToolType(Tool* tool)
{
	toolTypes.push_back(tool);
}

void CCTVManager::__AddSObjectType(SObject* sobj)
{
	sObjectTypes.push_back(sobj);

	if (FindContainer(sobj->GetOrder()) != nullptr)
		return;

	RenderOrderContainer* container = new RenderOrderContainer();
	container->orderID = sobj->GetOrder();

	FindContainerSpot(container);
	render_order_map[sobj->GetOrder()] = container;
}