#pragma once

#include "../Engine/Types/Texture2D.h"

class CCTVManager
{
public:
	static void OnInitialize();
	static void OnMoveableRender();
	static void OnStaticRender();
private:

	// Mode Switching Icons
	static Engine::Texture2D* editModeIcon;
	static Engine::Texture2D* cameraModeIcon;
	
	static bool isEditting;

	static void EditButtonToggle();
};