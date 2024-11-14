#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <stdexcept>

struct glfw_windowCreateInfo 
{
	const GLchar* title;
	GLint width;
	GLint height;
	GLFWmonitor* monitor;
};

class glfw_Window
{
	GLFWwindow* const window;

	GLfloat size[2];

	GLfloat scale;

public:

	glfw_Window(glfw_windowCreateInfo windowinfo);

	~glfw_Window();

	void run() const;
};

