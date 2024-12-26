#pragma once
#include <iostream>
#include <cassert>

namespace pronet {

#define UNSIGNED_INT_8	(8)
#define UNSIGNED_INT_16	(16)
#define UNSIGNED_INT_32	(32)
#define UNSIGNED_INT_64	(64)

	//	�r�b�g��`�悷��
	//	n : �`�悵�����ϐ�
	//	size : �ϐ��̃r�b�g��
	void print_bit(uint64_t n, uint8_t size);

	//	�ϐ��̃r�b�g�̍ŏ��̃r�b�g��0����������֐��A������Ȃ������� false ��Ԃ�
	//	n : �Ώۂ̕ϐ�
	//	size : �ϐ��̃r�b�g��
	//	start : �������n�߂�ʒu
	//	idx : �C���f�b�N�X���i�[����ϐ��A������Ȃ������瑀�삪�s���Ȃ�
	bool _bit_find_zero_from(uint64_t n, size_t size, size_t start, size_t* idx);

	//	�ϐ��̃r�b�g�̍ŏ��̃r�b�g��1����������֐��A������Ȃ������� false ��Ԃ�
	//	n : �Ώۂ̕ϐ�
	//	size : �ϐ��̃r�b�g��
	//	start : �������n�߂�ʒu
	//	idx : �C���f�b�N�X���i�[����ϐ��A������Ȃ������瑀�삪�s���Ȃ�
	bool _bit_find_one_from(uint64_t n, size_t size, size_t start, size_t* idx);

	//	�w�肵���̈�̃r�b�g�𒊏o����֐�
	//	n : ���o�������ϐ�
	//	size : �ϐ��̃r�b�g��
	//	start : ���o�������̈�̈ʒu
	//	area : ���o�������̈�̑傫��
	uint64_t _bit_extract_area(uint64_t n, size_t buf_size, size_t start, size_t area);

	//	�w�肵���̈��0����������
	//	n : �������݂����ϐ�
	//	size : �ϐ��̃r�b�g��
	//	start : �������݂����̈�̈ʒu
	//	area : �������݂����̈�̑傫��
	void _bit_write_zero_area(uint64_t* n, size_t buf_size, size_t start, size_t area);

	//	�w�肵���̈��1����������
	//	n : �������݂����ϐ�
	//	size : �ϐ��̃r�b�g��
	//	start : �������݂����̈�̈ʒu
	//	area : �������݂����̈�̑傫��
	void _bit_write_one_area(uint64_t* n, size_t buf_size, size_t start, size_t area);
}