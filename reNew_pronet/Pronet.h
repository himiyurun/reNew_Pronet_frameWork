#pragma once

#include "Shader.h"
#include "Object.h"
#include "TLSFmemory.h"
#include "glfw_Window.h"

static pronet::TLSFmemory tlsf(10, 8);

template<class T>
T* new_class(size_t size);

template<typename T>
void delete_class(void* p);

class PronetManager : public glfw_Window {
	Shader *shader;

	Object *object;

	const GLint dimentionSize;

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
		object[0].Init(dimentionSize, vertexcount, vertex);
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process() const;
};