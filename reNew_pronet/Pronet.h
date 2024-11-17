#pragma once
#include <memory>

#include "Shader.h"
#include "Object.h"
#include "glfw_Window.h"

class PronetManager : public glfw_Window {
	Shader shader;

	Object object;

	const GLint dimentionSize;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo windowInfo, GLint dimentionSize);

	//	�f�X�g���N�^
	~PronetManager();

	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader.Init(vsrc, fsrc);
	}

	void InitObj(GLsizei vertexcount, const glm::vec2* vertex) {
		object.Init(dimentionSize, vertexcount, vertex);
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process() const;
};