#pragma once

#ifndef SOBJECT_CAMERA
#define SOBJECT_CAMERA

#include "../CCTVManager.h"

class SCamera : public SObject
{
public:
	void OnSettings(glm::ivec2 screenSize);
	void OnDisplay();
	void OnSave();
	int GetID();
	int GetOrder();

	glm::vec2 position;
	std::string ip;

	bool validIP = false;
};

class CameraTool : public Tool
{
	const char* GetIcon();
	bool OnPlace(glm::vec2 cursor_position);
	void OnDisplay(glm::vec2 cursor_position);
};

#endif