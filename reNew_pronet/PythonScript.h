#pragma once
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

namespace py = boost::python;
class PythonScript
{
	py::object global;
	py::object local;
public:
	PythonScript();
	~PythonScript();

	bool execute_script(const char* _script);
	bool execute_file(const char* _file);
private:
	void init();
	void reset();
};

