#define PRONET_2D
#define _POOL_DEBUG
#define STRUCTURE_POOL_BIT_MAP_LEVEL	(6)
#define CHANCK_LOAD_SIZE	(1)

#include <bullet_db.hpp>
#include "Pronet.h"
#include "ObjectPoolTestor.h"

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

	ObjectPoolTestor<Object, 6> testor(32, 64, 30);
	testor.run();

	try {
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