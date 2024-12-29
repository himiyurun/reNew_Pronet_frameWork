#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <functional>

#include "Structure.h"

namespace pronet{
	class loadPronetMap2
	{
		std::ifstream ifs;
		std::string line;
		std::string script;
		std::istringstream iss;

		const char* file_name;
	public:
		//	�}�b�v�t�@�C����ǂݍ��݃X�g���N�`���̔z�u���擾����
		bool get_map_info(const char* name, std::unique_ptr<Structure2vCreateInfo>& info);

	private:
		//	�t�@�C�����J��
		bool read_file_init(const char* name);

		//	�t�@�C���̃^�C�v�������Ă��邩
		bool type_is();
		//	�t�@�C����ǂݍ���ł���ɉ������������s��
		void get_script(std::unique_ptr<Structure2vCreateInfo>& info);
		
		//	�t�@�C�������
		bool file_close();
		
		//	�w�肵���X�N���v�g�̏ꍇ���s���鏈�����L�q
		void script_func(const char* text, std::function<void()> func);
		//	string�֌W�̃����o�ϐ����N���A����
		void clear_string();
	};
}

