#pragma once
#include <stdexcept>

#include "pronet_memory.h"
#include "Object.h"
#include "Shader.h"
#include "Uniform.h"

//	�I�u�W�F�N�g��shared_ptr�Ǘ��ɂ���ΑS�ĉ���
//	���E���I�u�W�F�N�g�A��������`�����N�̃I�u�W�F�N�g�t�@�C���̃C���f�b�N�X�ɂ��I�u�W�F�N�g�����蓖�Ă�B
//	�m�[�}���I�u�W�F�N�g�A��������`�����N�̃I�u�W�F�N�g�t�@�C���̃C���f�b�N�X�ɂ��

struct Structure2vParamCreateInfo {
	GLfloat location[2];	//	�I�u�W�F�N�g�̈ʒu
	GLfloat col_pos[2];		//	�����蔻����Ƃ鍶��̃I�u�W�F�N�g���W
	GLfloat col_size[2];	//	�����蔻����Ƃ�T�C�Y
	GLfloat rotate;			//	�I�u�W�F�N�g�̊p�x
};

struct Structure2vCreateInfo {
	size_t shader_index;
	size_t buffer_object_index;
	size_t texture_index;
	Structure2vParamCreateInfo param;
};

template<std::size_t VBOLV, std::size_t SHDLV>
class Structure2v
{
	pronet::poolObject_shared_ptr<Object, VBOLV> buffer_object;	//	�o�b�t�@�I�u�W�F�N�g
	pronet::poolObject_shared_ptr<Shader, SHDLV> shader_object;	//	�V�F�[�_�[�̃C���f�b�N�X�A�`�����N���ƂɊǗ�
	uint32_t texture_index;	//	�e�N�X�`���̔ԍ�
	
	//	�p�����[�^�[
	Structure2v_Param param;
	GLfloat col_pos[2];		//	�����蔻����Ƃ鍶��̃I�u�W�F�N�g���W
	GLfloat col_size[2];	//	�����蔻����Ƃ�T�C�Y

public:
	//	�R���X�g���N�^
	Structure2v();
	//	�f�X�g���N�^
	~Structure2v();

	//	���������s��
	//	new_object : �m�ۂ����I�u�W�F�N�g�̃|�C���^
	void init(Structure2vParamCreateInfo* strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index);

	//	���̃N���X�������̏�Ԃɖ߂�
	void reset();
	//	�����̃p�����[�^�[���N���A����
	void clear();
	//	�V�F�[�_�[�g�p����
	void use() const;
	//	�`�悷��
	void draw() const;
	//	�g�p�������擾����
	bool is_used() const;

	[[nodiscard]] const Structure2v_Param* parameter() const { return &param; }
	[[nodiscard]] uint32_t texture() const { return (texture_index - 1); }
};

template<std::size_t VBOLV, std::size_t SHDLV>
Structure2v<VBOLV, SHDLV>::Structure2v()
	: buffer_object(nullptr)
	, shader_object(nullptr)
	, texture_index(0)
	, col_pos{ 0.0f, 0.0f }
	, col_size{ 0.0f, 0.0f }
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
Structure2v<VBOLV, SHDLV>::~Structure2v()
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::init(Structure2vParamCreateInfo* strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index)
{
	if (object() == nullptr)
		throw std::bad_alloc();
	if (shader() == nullptr)
		throw std::bad_alloc();
	if (tex_index == 0)
		throw std::runtime_error("tex_index is 0 Structure.update");

	//	�p�����[�^�[�̑��
	param.location[0] = strInfo->location[0]; param.location[1] = strInfo->location[1];
	param.rotate = strInfo->rotate;
	col_pos[0] = strInfo->col_pos[0]; col_pos[1] = strInfo->col_pos[1];
	col_size[0] = strInfo->col_size[0]; col_size[1] = strInfo->col_size[1];
	//	���̑��f�[�^�̑��
	buffer_object = object;
	shader_object = shader;
	texture_index = tex_index;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::reset()
{
	clear();
	buffer_object.reset();
	shader_object.reset();
	texture_index = 0;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::clear()
{
	//	�p�����[�^�[�̃��Z�b�g
	param.location[0] = 0.0f; param.location[1] = 0.0f;
	param.rotate = 0.0f;
	col_pos[0] = 0.0f; col_pos[1] = 0.0f;
	col_size[0] = 0.0f; col_size[1] = 0.0f;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::use() const
{
	shader_object->use();
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::draw() const
{
	buffer_object->draw();
}

template<std::size_t VBOLV, std::size_t SHDLV>
bool Structure2v<VBOLV, SHDLV>::is_used() const
{
	if (buffer_object == nullptr && shader_object == nullptr && texture_index == 0) {
		if ((param.location[0] == 0.0f && param.location[1] == 0.0f) &&
			(param.rotate == 0.0f) &&
			(col_pos[0] == 0.0f && col_pos[1] == 0.0f) &&
			(col_size[0] = 0.0f && col_size[1] = 0.0f)) {
			return false;
		}
		else {
			throw std::runtime_error("Structure reset not excute properly!!");
		}
	}
	return true;
}