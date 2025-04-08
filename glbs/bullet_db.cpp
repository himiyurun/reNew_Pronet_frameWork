#include "bullet_db.hpp"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>



BOOST_PYTHON_MODULE(glbs) {
	py::class_<std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>>("BulletParams")
		//.def(py::vector_indexing_suite<std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>>())
		.def("size", &std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>::size);
}