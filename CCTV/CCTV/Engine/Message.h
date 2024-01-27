#pragma once

#include <string>
#include <vector>

namespace Engine
{
	class Message
	{
	public:
		Message(std::string message);

		std::string GetMessageContent();
		void Push();

		static void FastMessage(std::string message);
		static void Render();
	private:
		std::string message;
		float transparency;

		static std::vector<Message> messages;
	};
}