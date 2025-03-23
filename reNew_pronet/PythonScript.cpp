#include "PythonScript.h"

PythonScript::PythonScript()
{
	Py_Initialize();
}

PythonScript::~PythonScript()
{
	Py_Finalize();
}

bool PythonScript::execute_script(const char* _script)
{
	try {
		init();
		py::exec(_script, global, local);
	}
	catch (const py::error_already_set&) {
		PyErr_Print();
		return false;
	}
	return true;
}

bool PythonScript::execute_file(const char* _file)
{
	try {
		init();
		py::exec(_file, global, local);
	}
	catch (const py::error_already_set&) {
		PyErr_Print();
		return false;
	}
	return true;
}

void PythonScript::init()
{
	global = py::import("__main__").attr("__dict__");
	local = py::import("__main__").attr("__dict__");
}

void PythonScript::reset()
{
	global = py::object();
	local = py::object();
}
