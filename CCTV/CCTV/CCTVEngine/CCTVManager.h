#pragma once

#include "../Engine/Types/Texture2D.h"
#include "../Engine/Macro.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <thread>

class Tool
{
public:
	virtual const char* GetIcon();
	virtual bool OnPlace(glm::vec2 cursor_position);
	virtual void OnDisplay(glm::vec2 cursor_position);
};

class SObject
{
public:
	virtual void OnDisplay();
	virtual void OnSave();
	virtual void OnSettings(glm::ivec2 screenSize);
	virtual int GetID();
	virtual int GetOrder();

	bool isValid = false;
};

class RenderOrderContainer
{
public:
	void Render();
	void Add(SObject* obj);
	void Remove(SObject* obj);
	int Contains(SObject* obj);

	int orderID;
	std::vector<SObject*> objs;
};

class CCTVManager
{
public:
	static void OnInitialize();
	static void OnDispose();
	static void OnMoveableRender();
	static void OnStaticRender();

	static bool IsSettingsOpen();
	static void SettingsToggle();

	static void AddSObject(SObject* obj);
	static void __AddToolType(Tool* tool);
	static void __AddSObjectType(SObject* sobj);

	static SObject* selected_object;

private:
	static std::vector<SObject*> sobjects;
	static std::vector<SObject*> sObjectTypes;
	static std::vector<RenderOrderContainer*> render_order;
	static std::unordered_map<int, RenderOrderContainer*> render_order_map;
	static std::vector<Tool*> toolTypes;
	static Tool* selected_tool;
	 
	// Mode Switching Icons
	static Engine::Texture2D* editModeIcon;
	static Engine::Texture2D* cameraModeIcon;
	static double settingsStartTime;


	static void ValidateThread();
	static std::thread validateThread;
	static bool IsMainThreadAlive;
	
	static bool isEditting;
	static bool isSettingsOpen;
	static float editButtonTime;

	static void UpdateCamera();
	
	static void EditButtonToggle();

	static void RenderSettings();
	static void SelectTool(Tool* tool);
	static void DeleteTool();

	static RenderOrderContainer* FindContainer(int orderID);
	static void FindContainerSpot(RenderOrderContainer* container);
};

#define ADD_TOOL(i) STATIC_EXECUTE{ CCTVManager::__AddToolType(i); }
#define ADD_SOBJECT(i) STATIC_EXECUTE{ CCTVManager::__AddSObjectType(i); }