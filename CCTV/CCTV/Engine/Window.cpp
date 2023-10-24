#include "Window.h"

using namespace Engine;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

}

// Creating a normal window.
bool Window::Init()
{
	// Sets up opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(480, 480, "Window", NULL, NULL);

	if (!_window)
		return false;

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	MakeCurrentContext();
	return true;
}

bool Window::Init(Window* window)
{
	// Sets up opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Attaches the opengl context to the target window.
	_window = glfwCreateWindow(1, 1, "Thread Context", NULL, window->Get());

	if (!_window)
		return false;
	return true;
}

void Window::Dispose()
{
	glfwDestroyWindow(_window);
}

GLFWwindow* Window::Get()
{
	return _window;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(_window);
}

void Window::MakeCurrentContext()
{
	glfwMakeContextCurrent(_window);
}

void Window::ClearNSwap()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SetSize(glm::ivec2 size)
{
	glfwSetWindowSize(_window, size.x, size.y);
}

glm::ivec2 Window::GetSize()
{
	glm::ivec2 size = glm::ivec2();
	glfwGetWindowSize(_window, &size.x, &size.y);
	return size;
}

void Window::SetWindowName(const char* name)
{
	glfwSetWindowTitle(_window, name);
}