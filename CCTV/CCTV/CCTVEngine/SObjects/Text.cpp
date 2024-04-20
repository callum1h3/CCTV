#include "Text.h"
#include "../../Engine/Renderer.h"

void SText::OnSettings(glm::ivec2 screenSize)
{

}

void SText::OnDisplay()
{
	Engine::Font font = Engine::Renderer::LoadFont(Engine::OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");

	if (CCTVManager::isEditting)
		Engine::Renderer::DrawTextInput(position, .1, &text, font, 64, "No Text");
	else
	{
		float width = font.GetTextWidth(.1, text);
		Engine::Renderer::DrawTextA(position - glm::vec2(width / 2, 0), glm::vec4(1, 1, 1, 1), .1, font, text);
	}
}

void SText::OnSave()
{

}

int SText::GetID()
{
	return 1;
}

int SText::GetOrder()
{
	return 2;
}

void SText::OnDispose()
{

}

const char* TextTool::GetIcon()
{
	return "text.png";
}

void TextTool::OnPickup()
{

}

bool TextTool::OnPlace(glm::vec2 cursor_position)
{
	SText* text = new SText();
	text->position = cursor_position;

	CCTVManager::AddSObject(text);
	return true;
}

void TextTool::OnDisplay(glm::vec2 cursor_position)
{
	Engine::Renderer::DrawRect(cursor_position, glm::vec2(4, 32), glm::vec4(1, 1, 1, 1));
}

ADD_TOOL(new TextTool());
ADD_SOBJECT(new SText());