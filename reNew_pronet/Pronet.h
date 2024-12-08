#pragma once

#include "Shader.h"
#include "Object.h"
#include "ObjectPool.h"
#include "glfw_Window.h"

class PronetManager : public glfw_Window, pnTlsf {
	Shader *shader;

	Object *object;

	const GLint dimentionSize;

	pronet::ObjectPool<Object> test;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	�f�X�g���N�^
	~PronetManager();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader[0].Init(vsrc, fsrc);
	}

	void InitObj(GLsizei vertexcount, const glm::vec2* vertex) {
		object->Init(dimentionSize, vertexcount, vertex);
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process() const;
};