#include "Camera.h"
#include "../../Engine/Renderer.h"

static Engine::Texture2D* camera_icon = nullptr;

bool CameraTool::OnPlace(glm::vec2 cursor_position)
{
	SCamera* camera = new SCamera();
	camera->ip = "";
	camera->position = cursor_position - glm::vec2(16, 16);

	CCTVManager::AddSObject(camera);

	return true;
}

void CameraTool::OnDisplay(glm::vec2 cursor_position)
{
	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	Engine::Renderer::DrawRect(cursor_position - glm::vec2(16, 16), glm::vec2(32, 32), glm::vec4(1, 1, 1, 1), camera_icon);
}


void SCamera::OnDisplay()
{
	if (camera_icon == nullptr)
		camera_icon = Engine::Resources::Load<Engine::Texture2D>("camera.png");

	Engine::Renderer::DrawRect(position, glm::vec2(32, 32), glm::vec4(1, 1, 1, 1), camera_icon);
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