#pragma once
#define BOOST_PYTHON_STATIC_LIB
#define PYTHON_RUN_BEGIN()	try {
#define PYTHON_RUN_END()		} catch (const boost::python::error_already_set& _e) { PyErr_Print(); return 1; }
#include <boost/python.hpp>
#include <iostream>
#include <string>

namespace py = boost::python;
class PythonScript
{
	py::object global_;
	py::object local_;

	std::string dir_;
public:
	PythonScript(const char* _dir = nullptr);
	~PythonScript();

	//	�t�@�C�����s�̎��̃t�@�C���̃f�B���N�g�����w�肷��
	//	dir : �f�B���N�g���p�X
	void setDir(const char* _dir);
	//	�X�N���v�g�����s
	//	script : �p�C�\���̃R�[�h�̕�����
	bool execute_script(const char* _script);
	//	.py �t�@�C�������s
	//	file : �t�@�C����
	bool execute_file(std::string _file);
	//	���[�J���p�C�\���I�u�W�F�N�g���擾
	[[nodiscard]] py::object operator[](const char* _name) const;
	//	�O���[�o���p�C�\���I�u�W�F�N�g���擾
	[[nodiscard]] py::object operator()(const char* _name) const;
private:
	void init();
	void reset();
};

