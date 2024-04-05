#include "Camera.h"
#include "../../Engine/Renderer.h"
#include "../../Engine/Input.h"

#define CAMERA_ICON_SIZE 64
#define CAMERA_ICON_SIZE_HALF CAMERA_ICON_SIZE / 2

static Engine::Texture2D* camera_icon = nullptr;

const char* CameraTool::GetIcon()
{
	return "camera.png";
}

bool CameraTool::OnPlace(glm::vec2 cursor_position)
{
	SCamera* camera = new SCamera();
	camera->ip = "";
	camera->position = cursor_position - glm::vec2(CAMERA_ICON_SIZE_HALF, CAMERA_ICON_SIZE_HALF);

	CCTVManager::AddSObject(camera);

	return true;
}

void CameraTool::OnDisplay(glm::vec2 cursor_position)
{
	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	Engine::Renderer::DrawRect(cursor_position - glm::vec2(CAMERA_ICON_SIZE_HALF, CAMERA_ICON_SIZE_HALF), glm::vec2(CAMERA_ICON_SIZE, CAMERA_ICON_SIZE), glm::vec4(1, 1, 1, 1), camera_icon);
}

void SCamera::OnSettings(glm::ivec2 screenSize)
{
	Engine::Font font = Engine::Renderer::LoadFont(Engine::OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");

	glm::vec2 middle = screenSize / 2;

	float ip_title_size = font.GetTextWidth(.1, "Device IP Address:");
	Engine::Renderer::DrawTextA(middle + glm::vec2(-(ip_title_size / 2), 60), glm::vec4(1, 1, 1, 1), .1, font, "Device IP Address:");


	Engine::Renderer::DrawTextInput(middle, .06, &ip, font, 64, "Please enter device IP");
}

void SCamera::OnDisplay()
{
	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	glm::vec2 size = glm::vec2(CAMERA_ICON_SIZE, CAMERA_ICON_SIZE);
	if (Engine::Renderer::IsMouseWithinBounds(position, position + size))
	{
		Engine::Renderer::DrawRect(position, size, glm::vec4(1, 1, 1, .2));

		if (Engine::Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT) && !CCTVManager::IsSettingsOpen())
		{
			CCTVManager::selected_object = this;
			CCTVManager::SettingsToggle();
		}
		
	}

	glm::vec4 col = glm::vec4(1, 1, 1, 1);
	if (!validIP)
		col = glm::vec4(1, 0, 0, 1);

	Engine::Renderer::DrawRect(position, size, col, camera_icon);
}

void SCamera::OnSave()
{

}

int SCamera::GetID()
{
	return 0;
}

// Making it so camera object displays ontop of everything.
int SCamera::GetOrder()
{
	return 1;
}

ADD_TOOL(new CameraTool());
ADD_SOBJECT(new SCamera());