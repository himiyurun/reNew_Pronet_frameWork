#include "PythonScript.h"

PythonScript::PythonScript(const char* _dir)
	: dir_(_dir)
{
	Py_Initialize();
}

PythonScript::~PythonScript()
{
	Py_Finalize();
}

void PythonScript::setDir(const char* _dir)
{
	dir_ = _dir;
}

bool PythonScript::execute_script(const char* _script)
{
	if (_script)
		throw std::runtime_error("script is null!! : PythonScript.execute_script(const char*)");
	try {
		init();
		py::exec(_script, global_, local_);
	}
	catch (const py::error_already_set&) {
		PyErr_Print();
		return false;
	}
	return true;
}

bool PythonScript::execute_file(std::string _file)
{
	try {
		init();
		std::string dirFile(dir_ + '/' + _file);
		py::exec_file(dirFile.c_str(), global_, local_);
	}
	catch (const py::error_already_set&) {
		PyErr_Print();
		return false;
	}
	return true;
}

py::object PythonScript::operator[](const char* _name) const
{
	return local_[_name];
}

py::object PythonScript::operator()(const char* _name) const
{
	return global_[_name];
}

void PythonScript::init()
{
	global_ = py::import("__main__").attr("__dict__");
	local_ = py::import("__main__").attr("__dict__");
}

void PythonScript::reset()
{
	global_ = py::object();
	local_ = py::object();
}
