#include "glfw_Window.h"

glfw_Window::glfw_Window(glfw_windowCreateInfo* windowinfo) noexcept
	: window(glfwCreateWindow(windowinfo->width, windowinfo->height, windowinfo->title, windowinfo->monitor, nullptr))
	, size{ static_cast<float>(windowinfo->width), static_cast<float>(windowinfo->height) }
	, scale(100.f), keyStatus(GLFW_RELEASE)
	, lastPosition{ 0.0f, 0.0f }, nowPosition{ 0.0f, 0.0f }
{
	if (window == nullptr) {
		std::cerr << "Error : window is null" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Error : Can't Load glad" << std::endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(1.f, 1.f, 1.f, 0.f);

	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, resize);

	glfwSetKeyCallback(window, keyBoard);

	glfwSetMouseButtonCallback(window, mouseButton);

	glfwSetScrollCallback(window, mouseWheel);

	glfwSetCursorPosCallback(window, Cursor);
}

glfw_Window::~glfw_Window()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void glfw_Window::bindWindowPram(GLuint sizeLoc, GLuint scaleLoc) const
{
	glUniform2fv(sizeLoc, 1, size);
	glUniform1f(scaleLoc, scale);
}

void glfw_Window::run() const
{
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		process();

		glfwSwapBuffers(window);

		if (keyStatus == GLFW_RELEASE)
			glfwWaitEvents();
		else
			glfwPollEvents();
	}
}

void glfw_Window::resize(GLFWwindow* window, int width, int height)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	GLint fbwidth = 0, fbheight = 0;
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);

	if (instance) {
		instance->size[0] = static_cast<GLfloat>(fbwidth);
		instance->size[1] = static_cast<GLfloat>(fbheight);
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}

void glfw_Window::keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->keyStatus = action;
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}

void glfw_Window::mouseButton(GLFWwindow* window, int button, int action, int mods)
{
}

void glfw_Window::mouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
			instance->scale += static_cast<GLfloat>(yoffset) * 100;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
			instance->scale += static_cast<GLfloat>(yoffset);
		else
			instance->scale += static_cast<GLfloat>(yoffset) * 5;
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}

void glfw_Window::Cursor(GLFWwindow* window, double xpos, double ypos)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		GLfloat xposf((xpos / (instance->size[0] / 2)) - 1.0f);
		GLfloat yposf(1.0f - (ypos / (instance->size[1] / 2)));
		instance->nowPosition[0] = xposf;
		instance->nowPosition[1] = yposf;
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}
