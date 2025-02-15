#define PRONET_2D
#define _POOL_DEBUG
#define STRUCTURE_POOL_BIT_MAP_LEVEL	(6)
#define CHANCK_LOAD_SIZE	(1)

#include "Pronet.h"
#include "tlsf.h"

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
	PronetManager<6, 6> game(&winInfo, "LoadFileList.fi");

	std::cout << "beg" << std::endl;
	pronet::BitMap64 bmp(76);
	bmp.write_Bit_1(0, 20);
	size_t i;
	bmp.write_Bit_1(43, 873);
	bmp.write_Bit_0(5, 10);
	bmp.write_Bit_0(343, 120);
	bmp.find_zero_from(45, &i);
	std::cout << "position : " << i << std::endl;
	bmp.find_one_from(5, &i);
	std::cout << "position : " << i << std::endl;
	bmp.printBit();
	std::cout << "end" << std::endl;
	/*
	pronet::tlsf_set test(4, 7);
	uint8_t fli, sli;
	test.calcIndex(fli, sli, 32);
	*/

	try {
		game.load();
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}