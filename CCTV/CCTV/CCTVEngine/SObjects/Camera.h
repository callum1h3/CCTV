#pragma once

#ifndef SOBJECT_CAMERA
#define SOBJECT_CAMERA

#include "../CCTVManager.h"

class SCamera : public SObject
{
public:
	void OnDisplay();
	void OnSave();
	int GetID();
	int GetOrder();

	glm::vec2 position;
	std::string ip;
};

class CameraTool : public Tool
{
	bool OnPlace(glm::vec2 cursor_position);
	void OnDisplay(glm::vec2 cursor_position);
};

#endif