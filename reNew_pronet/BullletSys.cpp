#include "BullletSys.h"

BulletSys::BulletSys(const char* _py_name)
	: pyObj_(_py_name ? py_obj::createPythonFunctionObject(_py_name) : 0)
	, bullet_(0)
	, exec_func_(nullptr)
	, gen_func_(nullptr)
	, upd_func_(nullptr)
{
	if (_py_name) {
		/*
		py::object set_up_func = py_obj::getExecuteFunction(pyObj_, "setUp")();
		py_obj::setUpInfo info = py::extract<setUpInfo>(set_up_func);
		*/
		exec_func_ = "execute";
		gen_func_ = "generate";
		upd_func_ = "update";
		bullet_ = bullet_system::createBulletDB("normal", 1.f, 16.f, 12, 10, 360.f, 0.07f, 0
			, bullet_system::_template::bullet_update_main, bullet_system::_template::bullet_generate_main);

	}
}
