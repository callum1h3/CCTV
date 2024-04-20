#pragma once

#ifndef SOBJECT_TEXT
#define SOBJECT_TEXT

#include "../CCTVManager.h"
#include "glm/glm.hpp"

class SText : public SObject
{
public:
	void OnSettings(glm::ivec2 screenSize);
	void OnDisplay();
	void OnSave();
	int GetID();
	int GetOrder();
	void OnDispose();

	std::string text;
	glm::vec2 position;
};

class TextTool : public Tool
{
public:
	void OnPickup();
	const char* GetIcon();
	bool OnPlace(glm::vec2 cursor_position);
	void OnDisplay(glm::vec2 cursor_position);
};

#endif