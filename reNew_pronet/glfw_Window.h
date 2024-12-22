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

typedef struct {
	GLfloat size[2];			//	ウインドウのサイズ
	GLfloat nowPosition[2];		//	マウスの現在の位置
	GLfloat lastPosition[2];	//	マウスの図形をつかんだときの位置（つかんでいないときは0）
	GLfloat scale;				//	ウインドウのスケール
} WindowParam;

class glfw_Window
{
	GLFWwindow* const window;

protected:

	WindowParam param;

	GLint keyStatus;

public:

	glfw_Window(glfw_windowCreateInfo *windowinfo) noexcept;

	~glfw_Window();

	//	ウインドウの情報をバインドする
	//	sizeLoc : ウインドウサイズの uniform変数の場所、vec2型
	//	scaleLoc : ウインドウスケールの uniform変数の場所、float型
	void bindWindowPram(GLuint sizeLoc, GLuint scaleLoc) const;

	void run();

	virtual void process()
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

