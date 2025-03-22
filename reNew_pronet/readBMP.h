#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <glad/glad.h>
#include "pnTlsf.h"

namespace pronet {
	class readBMP
	{
		const char* imgname_;
		BITMAPFILEHEADER fileh_;
		BITMAPINFOHEADER infoh_;
		std::vector<GLubyte, pnTlsfInsertSTLtype<GLubyte>> img_;
	public:
		//	�R���X�g���N�^
		//	imgname : �ǂݍ��ރr�b�g�}�b�v�摜�̖��O
		readBMP(const char* _imgname);
		//	�f�X�g���N�^
		~readBMP();
		//	�r�b�g�}�b�v�t�@�C���w�b�_�[���擾
		const BITMAPFILEHEADER& fileHead() const { return fileh_; }
		//	�r�b�g�}�b�v�C���t�H�w�b�_�[���擾
		const BITMAPINFOHEADER* operator->() const { return &infoh_; }
		//	�摜�̃s�N�Z���f�[�^���擾
		[[nodiscard]] const GLubyte* pixcel() const { return img_.data(); }
	};
}