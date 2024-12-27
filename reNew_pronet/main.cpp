
#define PRONET_2D
#define _POOL_DEBUG

#include "Pronet.h"
#include "Timer.h"
#include "bit_tree.h"

void libInit() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!glfwInit()) {
		std::cerr << "Error : Can't Initlize GLFW" << std::endl;
		exit(1);
	}
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
	PronetFrameWorkMain game(&winInfo, "LoadFileList.fi");

	Structure2vParamCreateInfo info;
	pronet::bit_tree<6> s(false, 120);
	size_t ss[12];
	for (int i = 0; i < 12; i++) {
		s.rigist(&ss[i]);
	}
	{
		Timer sec;
		for (int i = 0; i < 12; i++) {

			s.unrigist(ss[i]);
		}
	}
	for (int i = 0; i < 12; i++) {
		s.rigist(&ss[i]);
	}
	for (int i = 0; i < 12; i++) {
		Timer sec;
		s.unrigist_auto();
	}

	try {
		game.load(&info);
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}