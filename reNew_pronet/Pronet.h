#pragma once

#include "Shader.h"
#include "Object.h"
#include "TLSFmemory.h"
#include "glfw_Window.h"

class PronetManager : public glfw_Window {
	Shader *shader;

	Object *object;

	const GLint dimentionSize;

	pronet::TLSFmemory tlsf;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	�f�X�g���N�^
	~PronetManager();

	
	void InitShader(const char* vsrc, const char* fsrc) {
		if (shader) {
			uint8_t* ptr = reinterpret_cast<uint8_t*>(shader);
			Shader* buf = new(ptr) Shader();
			buf->Init(vsrc, fsrc);
		}
#ifdef _DEBUG
		else {
			throw std::runtime_error("Can't properly work Memory Allocater");
		}
#endif
	}

	void InitObj(GLsizei vertexcount, const glm::vec2* vertex) {
		if (object) {
			uint8_t* ptr = reinterpret_cast<uint8_t*>(object);
			Object* buf = new(ptr) Object();
			buf->Init(dimentionSize, vertexcount, vertex);
		}
#ifdef _DEBUG
		else {
			throw std::runtime_error("Can't properly work Memory Allocater");
		}
#endif
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process() const;
};