#define PRONET_2D
#define _POOL_DEBUG
#define STRUCTURE_POOL_BIT_MAP_LEVEL	(6)
#define CHANCK_LOAD_SIZE	(1)

#include <bullet_db.hpp>
#include "Pronet.h"
#include "tlsf.h"

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
			pronet::tlsf_set set(4, 32);
			pronet::tlsf_set::obj_type stra;
			pronet::tlsf_set::obj_type strb;
			pronet::tlsf_set::obj_type strc;
			pronet::tlsf_set::obj_type strd;
			set.printFlMap();
			set.get(128, stra);
			set.printFlMap();
			set.get(128, strb);
			set.back(stra);
			set.get(256, stra);
			set.get(256, strc);
			set.back(stra);
			set.get(512, stra);
			set.get(512, strd);
			set.back(stra);
			set.printFlMap();
			for (size_t i = 1; i <= 1024; i++) {
				std::cout << "allocate : " << i << std::endl;
				pronet::tlsf_set::obj_type str;
				if (!set.get(i, str)) {
					set.print_bmp();
					std::cerr << "error : tag is null : " << std::endl;
				}
				if (!str) {
					throw std::bad_alloc();
				}
				set.print_bmp();
				set.printFlMap();
				std::cout << "deallocate" << std::endl;
				set.back(str);
				set.print_bmp();
				set.printFlMap();
				std::cout << i << " end" << std::endl;
			}
			set.back(strb);
			set.back(strc);
			set.back(strd);
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