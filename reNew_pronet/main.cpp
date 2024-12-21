#include <memory>
#include <stdlib.h>
#include <crtdbg.h>

#define PRONET_2D

#include "Pronet.h"
#include "readDocument.h"

void libInit() {
	if (!glfwInit()) {
		std::cerr << "Error : Can't Initlize GLFW" << std::endl;
		exit(1);
	}
}

constexpr glm::vec2 rectangleVertex[] = {
	{ -0.5f,  0.5f },
	{ -0.5f, -0.5f },
	{  0.5f, -0.5f },
	{  0.5f,  0.5f }
};

constexpr GLuint rectangleIndex[] = {
	0, 1, 2, 0, 3, 2
};

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	libInit();

	glfw_windowCreateInfo winInfo = {};
	winInfo.width = 640;
	winInfo.height = 480;
	winInfo.title = "syu-thingumoruka-";
	winInfo.monitor = nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	PronetFrameWorkMain game(&winInfo, 2);

	ObjectInfo2v objInfo{};

	int size;
	pronet::PronetReadLoadFileList listfile("sample.fi", &size);
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo mainInfo(listfile.getLoadFile(0));

	try {
		game.InitShader(mainInfo.shaders[0][0].vsrc.c_str(), mainInfo.shaders[0][0].fsrc.c_str());
		game.InitObj(&mainInfo.objs[0][0], GL_TRUE);
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}