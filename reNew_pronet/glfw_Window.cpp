#include "glfw_Window.h"

glfw_Window::glfw_Window(glfw_windowCreateInfo windowinfo)
	: window(glfwCreateWindow(windowinfo.width, windowinfo.height, windowinfo.title, windowinfo.monitor, nullptr))
	, size{ static_cast<float>(windowinfo.width), static_cast<float>(windowinfo.height) }
	, scale(100.f)
{
	assert(window && "Error : window is null");

	glfwMakeContextCurrent(window);

	assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) && "Error : Can't initlize glad");

	glClearColor(1.f, 1.f, 1.f, 0.f);

	glfwSwapInterval(1);
}

glfw_Window::~glfw_Window()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void glfw_Window::run() const
{
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwWaitEvents();
	}
}
