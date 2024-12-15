#pragma once

#include "Shader.h"
#include "Object.h"
#include "ObjectPool.h"
#include "glfw_Window.h"

#define PronetFrameWorkMain FrameWorkManager

using FrameWorkManager = class PronetManager;

class PronetManager : public glfw_Window, pnTlsf {
	Shader *shader;

	Object *object;

	const GLint dimentionSize;

	pronet::ObjectPool<Object> test;

	pronet::ObjectPool<Shader> testshader;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	�f�X�g���N�^
	~PronetManager();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader = testshader.pop();
		if (shader) {
			shader->Init(vsrc, fsrc);
		}
	}

	void InitObj(ObjectInfo2v *info,GLboolean index_used) {
		object = test.pop();
		if (object) {
			object->Init(dimentionSize, info, index_used);
		}
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process() const;
};