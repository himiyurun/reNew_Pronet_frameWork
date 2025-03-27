#pragma once
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include "../reNew_pronet/PyBullet.h"
#include <glad/glad.h>

#define DLL_EXPORTS
#ifdef DLL_EXPORTS
#define GLBS_API __declspec(dllexport)
#else
#define GLBS_API __declspec(dllimport)
#endif

namespace py = boost::python;
namespace glbs {
	namespace database {
		namespace bullet {

		}
		namespace pool {

		}
	}
	namespace bullet {

	}
	namespace object {

	}
	namespace shader {

	}
	namespace texture {
	}
}