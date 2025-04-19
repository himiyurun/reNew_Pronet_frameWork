#pragma once
#include "readBMP.h"

class Texture
{
	GLuint tex;
	bool is_used;
public:
	//	�R���X�g���N�^
	//	imgname : �e�N�X�`���t�@�C����
	Texture(const char* _imgname = nullptr);
	//	�f�X�g���N�^
	~Texture();
	//	���������s��
	//	imgname : �e�N�X�`���t�@�C����
	void init(const char* _imgname);
	//	���Z�b�g����
	void reset();
	//	�e�N�X�`�����o�C���h����
	void bind() const;
	//	�e�N�X�`�����A�N�e�B�u�ɂ���
	virtual void active() const;
	//	�e�N�X�`�����g�p����
	//	id : �e�N�X�`�����j�b�g�̃��j�t�H�[���ϐ��̔ԍ�
	void exe(GLuint _id) const;
	//	�g�p�����̃X�e�[�^�X���擾
	[[nodiscard]] bool used() const { return is_used; }
	//	�e�N�X�`���I�u�W�F�N�g�̔ԍ����擾
	[[nodiscard]] GLuint obj() const { return tex; }
};

