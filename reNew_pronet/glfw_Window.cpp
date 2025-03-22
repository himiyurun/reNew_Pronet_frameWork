#include "glfw_Window.h"

glfw_Window::glfw_Window(glfw_windowCreateInfo* windowinfo) noexcept
	: window(glfwCreateWindow(windowinfo->width, windowinfo->height, windowinfo->title, windowinfo->monitor, nullptr))
	, keyStatus(GLFW_RELEASE), swapStatus(false)
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

	param.scale = 100.f;
    param.lastPosition[0] = 0.0f;
    param.lastPosition[1] = 0.0f;
	param.nowPosition[0] = 0.0f;
	param.nowPosition[1] = 0.0f;

	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, resize);

	glfwSetKeyCallback(window, keyBoard);

	glfwSetMouseButtonCallback(window, mouseButton);

	glfwSetScrollCallback(window, mouseWheel);

	glfwSetCursorPosCallback(window, Cursor);

	resize(window, windowinfo->width, windowinfo->height);
}

glfw_Window::~glfw_Window()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}

void glfw_Window::bindWindowPram(GLuint sizeLoc, GLuint scaleLoc) const
{
	glUniform2fv(sizeLoc, 1, param.size);
	glUniform1f(scaleLoc, param.scale);
}

void glfw_Window::setSwapEventsStatus(bool _status)
{
	swapStatus = _status;
}

void glfw_Window::run()
{
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		process();

		glfwSwapBuffers(window);

		if (swapStatus) {
			glfwPollEvents();
		}
		else {
			if (keyStatus == GLFW_RELEASE)
				glfwWaitEvents();
			else
				glfwPollEvents();
		}
	}
}

void glfw_Window::resize(GLFWwindow* window, int width, int height)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	GLint fbwidth = 0, fbheight = 0;
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	glViewport(0, 0, fbwidth, fbheight);

	if (instance) {
		instance->param.size[0] = static_cast<GLfloat>(fbwidth);
		instance->param.size[1] = static_cast<GLfloat>(fbheight);
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
			instance->param.scale += static_cast<GLfloat>(yoffset) * 100;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
			instance->param.scale += static_cast<GLfloat>(yoffset);
		else
			instance->param.scale += static_cast<GLfloat>(yoffset) * 5;
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}

void glfw_Window::Cursor(GLFWwindow* window, double xpos, double ypos)
{
	glfw_Window* const instance = static_cast<glfw_Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		GLfloat xposf((xpos / (instance->param.size[0] / 2)) - 1.0f);
		GLfloat yposf(1.0f - (ypos / (instance->param.size[1] / 2)));
		instance->param.nowPosition[0] = xposf;
		instance->param.nowPosition[1] = yposf;
	}
	else {
		throw std::runtime_error("Window user pointer is NULL");
	}
}
