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

	//	ファイル実行の時のファイルのディレクトリを指定する
	//	dir : ディレクトリパス
	void setDir(const char* _dir);
	//	スクリプトを実行
	//	script : パイソンのコードの文字列
	bool execute_script(const char* _script);
	//	.py ファイルを実行
	//	file : ファイル名
	bool execute_file(std::string _file);
	//	ローカルパイソンオブジェクトを取得
	[[nodiscard]] py::object operator[](const char* _name) const;
	//	グローバルパイソンオブジェクトを取得
	[[nodiscard]] py::object operator()(const char* _name) const;
private:
	void init();
	void reset();
};

