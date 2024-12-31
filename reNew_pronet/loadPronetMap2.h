#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <functional>

#include "Structure.h"

namespace pronet{
	enum loadChankDirection {
		CHANCK_NATIVE,
		CHANCK_BOUNDARY_UP,
		CHANCK_BOUNDARY_DOWN,
		CHANCK_BOUNDARY_RIGHT,
		CHANCK_BOUNDARY_LEFT,
		CHANCK_DIRECTION_SIZE
	};

	class loadPronetMap2
	{
		std::ifstream ifs;
		std::string line;
		std::string script;
		std::istringstream iss;

		const char* file_name;

		static pronet::ObjectPool_Array<Structure2vCreateInfo> pool;

	public:
		loadPronetMap2();

		//	�}�b�v�t�@�C����ǂݍ��݃X�g���N�`���̔z�u���擾����
		bool get_mapInfo(const char* name, poolArray_unique_ptr<Structure2vCreateInfo> info[5]);

		//	�X�g���N�`���̔z�u�̏����v�[���ɕԋp����
		void return_create_info(PoolArray<Structure2vCreateInfo> info[5]);

	private:
		//	�t�@�C�����J��
		bool read_file_init(const char* name);
		//	�t�@�C���̃^�C�v�������Ă��邩
		bool type_is();
		//	�X�g���N�`���֐����̒������s��
		void get_struct(poolArray_unique_ptr<Structure2vCreateInfo>& info);
		//	�X�g���N�`���̊��蓖�Ă��s��
		void structure_by_script(const char* text, poolArray_unique_ptr<Structure2vCreateInfo>& info);
		//	�s���Ƃɕ�����ǂݍ���
		void line_getting_by_text();
		//	�t�@�C����ǂݍ���ł���ɉ������������s��
		void get_script(poolArray_unique_ptr<Structure2vCreateInfo> info[5]);
		
		//	�t�@�C�������
		bool file_close();
		
		//	�w�肵���X�N���v�g�̏ꍇ���s���鏈�����L�q
		void script_func(const char* text, std::function<void()> func);
		//	string�֌W�̃����o�ϐ����N���A����
		void clear_string();
	};
}

