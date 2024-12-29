#pragma once
#include "pronet_memory.h"
#include "ObjectPoolList.h"
#include "Structure.h"
#include "Shader.h"

namespace pronet {
	/*
	* ���E���܂����I�u�W�F�N�g�̏���
	* �J�X�^���X�}�[�g�|�C���^�̎g�p�c4�������̃��X�g���쐬������������ʂ̑����A�X�}�[�g�|�C���^�����������Ӗ�
	* ���P��
	* �E�X�g���N�`�����X�}�[�g�|�C���^�ŊǗ������ɂ��Ȃ��Ă��邪�I�u�W�F�N�g�̈������ǂ����悤���P��̃I�u�W�F�N�g�v�[���̎���
	* 
	* �X�g���N�`���[
	* �E�I�u�W�F�N�g�E�E�X�}�[�g�|�C���^
	* �E�V�F�[�_�E�E�X�}�[�g�|�C���^
	* �E�e�N�X�`���E�E�C���f�b�N�X
	* �E�p�����E�E��
	* 
	* ����
	* �E���[�h���ɃX�}�[�g�|�C���^�𐶐�
	* �E�Q�Ƃ���B���I�u�W�F�N�g�͐����͈͂��O��邽�ߍ폜�������������̂͊m�ۂ��ꂽ�܂�
	*/

	class Chanck_2D
	{
		PoolArray<Structure2v> structure;
		PoolArray<Object> buffer_pool;
		PoolArray<Shader> shader;
	
	public:
		Chanck_2D();

		~Chanck_2D();
	};
}

