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
	GLfloat size[2];			//	�E�C���h�E�̃T�C�Y
	GLfloat nowPosition[2];		//	�}�E�X�̌��݂̈ʒu
	GLfloat lastPosition[2];	//	�}�E�X�̐}�`�����񂾂Ƃ��̈ʒu�i����ł��Ȃ��Ƃ���0�j
	GLfloat scale;				//	�E�C���h�E�̃X�P�[��
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

	//	�E�C���h�E�̏����o�C���h����
	//	sizeLoc : �E�C���h�E�T�C�Y�� uniform�ϐ��̏ꏊ�Avec2�^
	//	scaleLoc : �E�C���h�E�X�P�[���� uniform�ϐ��̏ꏊ�Afloat�^
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

