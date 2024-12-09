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

	GLint keyStatus;

	GLfloat lastPosition[2];

	GLfloat nowPosition[2];

public:

	glfw_Window(glfw_windowCreateInfo *windowinfo) noexcept;

	~glfw_Window();

	//	ウインドウの情報をバインドする
	//	sizeLoc : ウインドウサイズの uniform変数の場所、vec2型
	//	scaleLoc : ウインドウスケールの uniform変数の場所、float型
	void bindWindowPram(GLuint sizeLoc, GLuint scaleLoc) const;

	void run() const;

	virtual void process() const
	{
		std::cout << "Hello World!!" << std::endl;
	};

private:

	static void resize(GLFWwindow* window, int width, int height);

	static void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouseButton(GLFWwindow* window, int button, int action, int mods);

	static void mouseWheel(GLFWwindow* window, double xoffset, double yoffset);

	static void Cursor(GLFWwindow* window, double xpos, double ypos);
};

