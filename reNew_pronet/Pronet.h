#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPoolArray.h"
#include "glfw_Window.h"

#define PronetFrameWorkMain FrameWorkManager

using FrameWorkManager = class PronetManager;

//	�E�C���h�E�̃p�����[�^�[�𑗐M���郆�j�t�H�[���o�b�t�@�I�u�W�F�N�g

class PronetManager : public glfw_Window, pnTlsf {
	pronet::PoolArray<Shader> shader;
	pronet::PoolArray<Object> object;

	GLint dimentionSize;

	pronet::ObjectPool_Array<Object> objPool;
	pronet::ObjectPool_Array<Shader> shdPool;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	�f�X�g���N�^
	~PronetManager();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader[0].init(vsrc, fsrc);
	}

	void InitObj(ObjectInfo2v *info,GLboolean index_used) {
		object[0].init(dimentionSize, info, index_used);
	}

	//	���[�v���Ŏ��s���鏈��
	virtual void process();
};