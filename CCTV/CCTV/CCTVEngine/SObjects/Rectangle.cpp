#include "Rectangle.h"
#include "../../Engine/Renderer.h"
#include <iostream>

static glm::vec4 RECT_COLOR = glm::vec4(0.05f, 0.05f, 0.05f, 1);

void SRectangle::OnSettings(glm::ivec2 screenSize)
{

}

void SRectangle::OnDisplay()
{
	Engine::Renderer::DrawRect(position, size, RECT_COLOR);
}

void SRectangle::OnSave()
{

}

int SRectangle::GetID()
{
	return 1;
}

int SRectangle::GetOrder()
{
	return 3;
}

void SRectangle::OnDispose()
{

}

const char* RectangleTool::GetIcon()
{
	return "rectangle.png";
}

void RectangleTool::OnPickup()
{
	__hasPlaced = false;
}

bool RectangleTool::OnPlace(glm::vec2 cursor_position)
{
	if (!__hasPlaced)
	{
		startPosition = cursor_position;
		__hasPlaced = true;

		return false;
	}

	SRectangle* rect = new SRectangle();
	rect->position = startPosition;
	rect->size = cursor_position - startPosition;

	CCTVManager::AddSObject(rect);

	return true;
}

void RectangleTool::OnDisplay(glm::vec2 cursor_position)
{
	if (__hasPlaced)
	{
		double x, y;
		Engine::Renderer::GetMouseMoveable(x, y);
		glm::vec2 cursor_mov = glm::vec2(x, y);

		glm::vec2 size = startPosition - cursor_mov;
		Engine::Renderer::DrawRect(cursor_position, size, RECT_COLOR);

		return;
	}

	Engine::Renderer::DrawRect(cursor_position, glm::vec2(4,4), RECT_COLOR);
}

ADD_TOOL(new RectangleTool());
ADD_SOBJECT(new SRectangle());