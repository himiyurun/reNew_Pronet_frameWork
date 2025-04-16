#pragma once
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <glbs/py_db.hpp>

#define DLL_EXPORTS
#ifdef DLL_EXPORTS
#define GLBS_API __declspec(dllexport)
#else
#define GLBS_API __declspec(dllimport)
#endif

namespace py = boost::python;

GLBS_API void printBulletParamData(const data_base::BULLET_PARAM_DATA& _str);

GLBS_API void test(float _pos[2]);

BOOST_PYTHON_MODULE(glbs) {
	using namespace data_base;
	using namespace py_obj;
	using namespace bullet_system;
	py::def("test", &test);
	py::def("getLastClock", &data_base::getLastClock);
	py::class_<BULLET_PARAM_DATA>("BulletParamData")
		.def_readwrite("coef", &BULLET_PARAM_DATA::coef)
		.def_readwrite("area", &BULLET_PARAM_DATA::area)
		.def_readwrite("count", &BULLET_PARAM_DATA::count)
		.def_readwrite("interval", &BULLET_PARAM_DATA::interval)
		.def_readwrite("rad", &BULLET_PARAM_DATA::rad)
		.def_readwrite("speed", &BULLET_PARAM_DATA::speed);
	py::class_<BULLET_OBJECT_PARAM>("BulletObjectParam")
		.def_readwrite("posX", &BULLET_OBJECT_PARAM::positionX)
		.def_readwrite("posY", &BULLET_OBJECT_PARAM::positionY)
		.def_readwrite("deg", &BULLET_OBJECT_PARAM::deg);
	py::class_<BULLET_OBJECT_DATA>("BulletObjectData")
		.def_readwrite("posX", &BULLET_OBJECT_DATA::positionX)
		.def_readwrite("posY", &BULLET_OBJECT_DATA::positionY)
		.def_readwrite("begDeg", &BULLET_OBJECT_DATA::begin_degree)
		.def_readwrite("rad", &BULLET_OBJECT_DATA::radiculus);
	py::def("printBulletParamData", &printBulletParamData);
	py::def("createPythonSetupInfo", &createPythonSetupInfo);
	py::def("createBulletObject", &createBulletObjectByPython);
}
