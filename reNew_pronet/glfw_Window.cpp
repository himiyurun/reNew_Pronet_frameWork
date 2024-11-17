#include "glfw_Window.h"

glfw_Window::glfw_Window(glfw_windowCreateInfo* windowinfo)
	: window(glfwCreateWindow(windowinfo->width, windowinfo->height, windowinfo->title, windowinfo->monitor, nullptr))
	, size{ static_cast<float>(windowinfo->width), static_cast<float>(windowinfo->height) }
	, scale(100.f)
{
	if (window == nullptr) {
		std::cerr << "window is null" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Can't Load glad" << std::endl;
		exit(EXIT_FAILURE);
	}

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

		process();

		glfwSwapBuffers(window);

		glfwWaitEvents();
	}
}
