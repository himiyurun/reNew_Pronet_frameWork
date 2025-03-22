#pragma once
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Uniform.h"

struct glfw_windowCreateInfo 
{
	const char* title;
	int width;
	int height;
	GLFWmonitor* monitor;
};

class glfw_Window
{
protected:
	GLFWwindow* const window;
	WindowParam param;
	GLint keyStatus;
	GLboolean swapStatus;

public:

	glfw_Window(glfw_windowCreateInfo *windowinfo) noexcept;

	~glfw_Window();

	//	ウインドウの情報をバインドする
	//	sizeLoc : ウインドウサイズの uniform変数の場所、vec2型
	//	scaleLoc : ウインドウスケールの uniform変数の場所、float型
	void bindWindowPram(GLuint sizeLoc, GLuint scaleLoc) const;

	void setSwapEventsStatus(bool _status);

	void run();

	virtual void process()
	{
		std::cout << "Hello World!!" << std::endl;
	};

	[[nodiscard]] bool swapStat() const { return swapStatus; }

private:

	static void resize(GLFWwindow* window, int width, int height);

	static void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouseButton(GLFWwindow* window, int button, int action, int mods);

	static void mouseWheel(GLFWwindow* window, double xoffset, double yoffset);

	static void Cursor(GLFWwindow* window, double xpos, double ypos);
};

