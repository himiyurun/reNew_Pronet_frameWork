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
	//	�e�N�X�`���I�u�W�F�N�g�̔ԍ����擾
	[[nodiscard]] GLuint obj() const { return tex; }
};

