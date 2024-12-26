#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPoolArray.h"
#include "glfw_Window.h"
#include "readDocument.h"
#include "Structure.h"

#define PronetFrameWorkMain Pronet_Manager_main

using Pronet_Manager_main = class PronetManager;

//	�E�C���h�E�̃p�����[�^�[�𑗐M���郆�j�t�H�[���o�b�t�@�I�u�W�F�N�g
class PronetManager : public glfw_Window, pnTlsf {
	pronet::PronetReadLoadFileList file_reader;

	GLint dimentionSize;

	pronet::PoolArray<Shader> shader;
	pronet::PoolArray<Object> object;
	pronet::ObjectPool_Array<Object> objPool;
	pronet::ObjectPool_Array<Shader> shdPool;
	pronet::ObjectPool_Array<glm::vec2> vertsPool;
	pronet::ObjectPool_Array<uint32_t> indexPool;
	Structure2v str;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	�R���X�g���N�^
	//	windowInfo : �쐬����E�C���h�E�̏��
	//	dimentionSize : �쐬����Q�[���̎���
	PronetManager(glfw_windowCreateInfo *windowInfo, const char* loadfilelist_name);

	//	�f�X�g���N�^
	~PronetManager();

	void load(Structure2vParamCreateInfo* strInfo);
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader[0].init(vsrc, fsrc);
	}

	void InitObj() {
		str.init(&object[0]);
	}

	void initStr(Structure2vParamCreateInfo* strInfo, ObjectInfo2v* objInfo, uint32_t shd_index, uint32_t tex_index);

	//	���[�v���Ŏ��s���鏈��
	virtual void process();
};