#include "Input.h"
#include "../Application.h"

using namespace Engine;

Window* Input::window;
bool Input::isFirstFPSStyle = true;

void Input::Initialize()
{
	window = Application::GetWindow();
}

int Input::GetKey(int key)
{
	return glfwGetKey(window->Get(), key);
}

void Input::GetCursor(double* x, double* y)
{
	glfwGetCursorPos(window->Get(), x, y);
}

int Input::GetMouse(int key)
{
	return glfwGetMouseButton(window->Get(), key);
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
