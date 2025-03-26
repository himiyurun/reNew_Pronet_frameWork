#include "bullet_db.hpp"

GLBS_API void Loop(py::object _func, std::size_t _ct)
{
	for (size_t i = 0; i < _ct; i++) {
		_func();
	}
}

BOOST_PYTHON_MODULE(glbs) {
	py::def("Loop", &Loop);
}