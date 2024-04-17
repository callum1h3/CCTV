#pragma once

#ifndef SOBJECT_CAMERA
#define SOBJECT_CAMERA

#include "../CCTVManager.h"
#include "jpeg-compressor/jpge.h"

#ifndef _WINSOCK2API_
#define _WINSOCK2API_
#define _WINSOCKAPI
#endif

#include <winsock2.h>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class SCamera;

struct ImageData
{
public:
	SCamera* camera;

	int width, height;
	const unsigned char* data;
};

class SCamera : public SObject
{
public:
	void OnSettings(glm::ivec2 screenSize);
	void OnDisplay();
	void OnSave();
	int GetID();
	int GetOrder();

	void Validate();
	
	void GenerateImage();
	bool HasImage();
	bool ShouldBeGenerating(std::string target_ip);

	static SOCKET ConnectToImageStream(std::string target_ip);
	static void GatherImageThread(SCamera* target, std::string target_ip);

	Engine::Texture2D* GetTexture();

	glm::vec2 position;
	std::string lastCheckedIP;
	std::string ip;

	Engine::Texture2D* camera_texture;
	bool isRetrievingCamera = false;
	int camera_width, camera_height;
	double lastRequestTime = -100;
	std::thread camera_thread;
	ImageData* queuedData = nullptr;


	bool validIP = false;
	bool validCamera = false;
	bool errorWithCamera = false;

	float sshCooldown = 0;

	bool IsUseable();
};

class CameraTool : public Tool
{
	const char* GetIcon();
	bool OnPlace(glm::vec2 cursor_position);
	void OnDisplay(glm::vec2 cursor_position);
};

#endif