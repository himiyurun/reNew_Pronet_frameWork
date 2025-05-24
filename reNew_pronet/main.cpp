#define PRONET_2D
#define _POOL_DEBUG
#define STRUCTURE_POOL_BIT_MAP_LEVEL	(6)
#define CHANCK_LOAD_SIZE	(1)

#include <bullet_db.hpp>
#include "Pronet.h"
#include "tlsf.h"

class sample_class {
	int i;
public:
	sample_class(int i = 0)
		: i(i)
	{
	}
	void set(int _i) { i = _i; }
	[[nodiscard]] int get() const { return i; }
};

std::weak_ptr<sample_class> ptrhoge;

void libInit() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!glfwInit()) {
		std::cerr << "Error : Can't Initlize GLFW" << std::endl;
		exit(1);
	}
	atexit(glfwTerminate);

	bullet_system::BulletSystemInit();
	atexit(bullet_system::BulletSystemTerminate);
	PyInit_glbs();
}

void testfun(std::weak_ptr<sample_class> sp) {
	ptrhoge = sp;
}

int main() {
	libInit();
	
	glfw_windowCreateInfo winInfo = {};
	winInfo.width = 640;
	winInfo.height = 480;
	winInfo.title = "test_game";
	winInfo.monitor = nullptr;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	PronetManager<6, 6> game(&winInfo, "LoadFileList.fi");

	try {
		{
			pronet::tlsf_set set(4, 1024);
			for (size_t i = 0; i < 1024; i++) {
				pronet::tlsf_set::obj_type str;
				if (!set.get(i, str))
					std::cerr << "error : " << std::endl;
				if (!str)
					throw std::bad_alloc();
				set.print_bmp();
				set.printFlMap();
				set.back(str);
				set.print_bmp();
				set.printFlMap();
				std::cout << i << " end" << std::endl;
			}
		}
		game.load();
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		std::cout << "プログラムを終了するには Enter キーを押してください" << std::endl;
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}