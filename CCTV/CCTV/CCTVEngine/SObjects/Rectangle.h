#pragma once

#ifndef SOBJECT_RECTANGLE
#define SOBJECT_RECTANGLE

#include "../CCTVManager.h"
#include "glm/glm.hpp"

class SRectangle : public SObject
{
public:
	void OnSettings(glm::ivec2 screenSize);
	void OnDisplay();
	void OnSave();
	int GetID();
	int GetOrder();
	void OnDispose();

	glm::vec2 position;
	glm::vec2 size;
};

class RectangleTool : public Tool
{
public:
	void OnPickup();
	const char* GetIcon();
	bool OnPlace(glm::vec2 cursor_position);
	void OnDisplay(glm::vec2 cursor_position);

	glm::vec2 startPosition;
	bool __hasPlaced = false;
};

#endif