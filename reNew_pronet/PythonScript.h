#pragma once
#define BOOST_PYTHON_STATIC_LIB
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

	void setDir(const char* _dir);
	bool execute_script(const char* _script);
	bool execute_file(std::string _file);

	[[nodiscard]] py::object operator[](const char* _name) const;
	[[nodiscard]] py::object operator()(const char* _name) const;
private:
	void init();
	void reset();
};

