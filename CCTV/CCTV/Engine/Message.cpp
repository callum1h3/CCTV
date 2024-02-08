#include "Message.h"
#include "Renderer.h"
#include "OSHandler.h"
#include "MathHelper.h"
#include "Time.h"

using namespace Engine;

std::vector<Message> Message::messages;

Message::Message(std::string message)
{
	this->message = message;
	this->transparency = 15;
}

std::string Message::GetMessageContent()
{
	return message;
}

void Message::Push()
{
	messages.push_back(*this);
}

void Message::FastMessage(std::string message)
{
	Message m = Message(message);
	m.Push();
}

void Message::Render()
{
	float font_size = 0.05f;

	Font font = Renderer::LoadFont(OSHandler::GetPath() + "/fonts/FiraCode-SemiBold.ttf");
	float text_height = font.height * font_size;

	for (int i = 0; i < 5; i++)
	{
		if (Message::messages.size() <= i)
			break;

		Message* message = &Message::messages[i];

		float font_width = font.GetTextWidth(font_size, message->GetMessageContent());
		float y_offset = (text_height + 12) * i;

		glm::vec2 position = glm::vec2(8, Renderer::ScreenSize().y - y_offset) - glm::vec2(0, text_height + 12);

		Renderer::DrawRect(position + glm::vec2(-4,-4), glm::vec2(font_width + 8, text_height + 8), glm::vec4(0.01f, 0.01f, 0.01f, message->transparency));
		Renderer::DrawTextA(position, glm::vec4(1.0f, 1.0f, 1.0f, message->transparency), font_size, font, message->GetMessageContent());
		

		if (i == 0)
			message->transparency -= Time::DeltaTime() * 2.0f;
	}

	if (Message::messages.size() > 0)
		if (Message::messages[0].transparency < 0.01)
			Message::messages.erase(Message::messages.begin());
}

