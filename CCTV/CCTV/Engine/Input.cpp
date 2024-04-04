#include "Input.h"
#include "Renderer.h"
#include "../Application.h"
#include <iostream>

using namespace Engine;

Window* Input::window;
bool Input::isFirstFPSStyle = true;
std::unordered_map<int, Key*> Input::keys;
std::vector<Key*> Input::keys_vector;


void Input::Initialize()
{
	window = Application::GetWindow();

	glfwSetKeyCallback(window->Get(), KeyCallback);
	glfwSetMouseButtonCallback(window->Get(), MouseButtonCallback);
}

void Input::LateUpdate()
{
	for (Key* key : keys_vector)
		key->pressedThisFrame = false;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;

	if (!keys.count(key))
	{
		Key* new_key = new Key();
		keys[key] = new_key;
		keys_vector.push_back(new_key);
	}
	
	Key* _key = keys[key];
	_key->isUp = (action == GLFW_RELEASE);
	_key->pressedThisFrame = true;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	KeyCallback(window, -(button + 1), 0, action, mods);
}

bool Input::GetKey(int key)
{
	if (!Renderer::isMouseEnabled)
		return false;

	if (!keys.count(key))
		return false;

	return !keys[key]->isUp;
}

bool Input::IsKeyDown(int key)
{
	if (!Renderer::isMouseEnabled)
		return false;

	if (!keys.count(key))
		return false;

	Key* _key = keys[key];
	return !_key->isUp && _key->pressedThisFrame;
}

bool Input::IsKeyUp(int key)
{
	if (!keys.count(key))
		return false;

	Key* _key = keys[key];
	return _key->isUp && _key->pressedThisFrame;
}

bool Input::GetMouse(int key)
{
	if (!Renderer::isMouseEnabled)
		return false;

	return GetKey(-(key + 1));
}

bool Input::IsMouseDown(int key)
{
	if (!Renderer::isMouseEnabled)
		return false;
	return IsKeyDown(-(key + 1));
}

bool Input::IsMouseUp(int key)
{
	if (!Renderer::isMouseEnabled)
		return false;
	return IsKeyUp(-(key + 1));
}

void Input::GetCursor(double* x, double* y)
{
	glfwGetCursorPos(window->Get(), x, y);
}

void Input::SetInputMode(int state)
{
	glfwSetInputMode(window->Get(), GLFW_CURSOR, state);

	if (state == GLFW_CURSOR_NORMAL)
		isFirstFPSStyle = true;
}

void Input::GetMouseFPSStyle(double* x, double* y)
{
	glm::ivec2 size = window->GetSize();
	int new_x = size.x / 2;
	int new_y = size.y / 2;

	double mousex, mousey;
	glfwGetCursorPos(window->Get(), &mousex, &mousey);

	glfwSetCursorPos(window->Get(), new_x, new_y);

	if (isFirstFPSStyle)
	{
		isFirstFPSStyle = false;
		*x = 0;
		*y = 0;
		return;
	}

	*x = mousex - new_x;
	*y = mousey - new_y;
}
