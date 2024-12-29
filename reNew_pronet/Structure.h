#pragma once
#include <stdexcept>

#include "ObjectPoolList.h"
#include "Object.h"

//	�I�u�W�F�N�g��shared_ptr�Ǘ��ɂ���ΑS�ĉ���
//	���E���I�u�W�F�N�g�A��������`�����N�̃I�u�W�F�N�g�t�@�C���̃C���f�b�N�X�ɂ��I�u�W�F�N�g�����蓖�Ă�B
//	�m�[�}���I�u�W�F�N�g�A��������`�����N�̃I�u�W�F�N�g�t�@�C���̃C���f�b�N�X�ɂ��

struct Structure2vCreateInfo {
	size_t buffer_object_index;
	size_t shader_index;
	Structure2vParamCreateInfo param;
};

struct Structure2vParamCreateInfo {
	GLfloat location[2];	//	�I�u�W�F�N�g�̈ʒu
	GLfloat col_pos[2];		//	�����蔻����Ƃ鍶��̃I�u�W�F�N�g���W
	GLfloat col_size[2];	//	�����蔻����Ƃ�T�C�Y
	GLfloat rotate;			//	�I�u�W�F�N�g�̊p�x
};

struct Structure2v_Param {
	GLfloat location[2];	//	�I�u�W�F�N�g�̈ʒu
	GLfloat rotate;			//	�I�u�W�F�N�g�̊p�x

	Structure2v_Param() : location{ 0.0f, 0.0f }, rotate(0.0f) {}
};

class Structure2v
{
	Object* buffer_object;	//	�o�b�t�@�I�u�W�F�N�g
	uint32_t shader_index;	//	�V�F�[�_�[�̃C���f�b�N�X�A�`�����N���ƂɊǗ�
	uint32_t texture_index;	//	�e�N�X�`���̔ԍ�
	
	//	�p�����[�^�[
	Structure2v_Param param;
	GLfloat col_pos[2];		//	�����蔻����Ƃ鍶��̃I�u�W�F�N�g���W
	GLfloat col_size[2];	//	�����蔻����Ƃ�T�C�Y
	pronet::PoolArray<glm::vec2> verts;
	pronet::PoolArray<glm::vec2> uv;
	pronet::PoolArray<uint32_t> indics;

public:
	//	�R���X�g���N�^
	Structure2v();
	//	�f�X�g���N�^
	~Structure2v();

	//	���������s��
	//	new_object : �m�ۂ����I�u�W�F�N�g�̃|�C���^
	void init(Object* new_object);
	//	�����̃p�����[�^������������
	//	info : �I�u�W�F�N�g�̏��
	//	shd_index : �V�F�[�_�[�̃C���f�b�N�X
	//	tex_index : �e�N�X�`���̃C���f�b�N�X
	void update(Structure2vParamCreateInfo *strInfo, ObjectInfo2v* objInfo, uint32_t shd_index, uint32_t tex_index);
	//	���̃N���X�������̏�Ԃɖ߂�
	void reset();
	//	�����̃p�����[�^�[���N���A����
	void clear();
	//	�`�悷��
	void draw() const;

	[[nodiscard]] uint32_t shader() const { return (shader_index - 1); }
	[[nodiscard]] uint32_t texture() const { return (texture_index - 1); }
};

