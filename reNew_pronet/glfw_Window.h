#pragma once
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct glfw_windowCreateInfo 
{
	const char* title;
	int width;
	int height;
	GLFWmonitor* monitor;
};

class glfw_Window
{
	GLFWwindow* const window;

protected:

	GLfloat size[2];

	GLfloat scale;

public:

	glfw_Window(glfw_windowCreateInfo *windowinfo);

	~glfw_Window();

	void run() const;

	virtual void process() const{
		std::cout << "Hello World!!" << std::endl;
	};
};

