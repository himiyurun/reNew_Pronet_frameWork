#include <memory>
#include <stdlib.h>
#include <crtdbg.h>

#define PRONET_2D
#define _POOL_DEBUG

#include "Pronet.h"
#include "readDocument.h"
#include "ObjectPoolArray.h"

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

	ObjectInfo2v objInfo{};
	pronet::ObjectPool_Array<Object> test;
	test.resize(12*64);
	pronet::PoolArray<Object> hoge1 = test.pop(320);
	pronet::PoolArray<Object> hoge2 = test.pop(248);
	pronet::PoolArray<Object> hoge3 = test.pop(480);
	test.push(&hoge1);

	//test.pop(64 * 4);
	//test.pop(64 * 8);

	int size;
	pronet::PronetReadLoadFileList listfile("sample.fi", &size);
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo mainInfo(listfile.getLoadFile(0));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	PronetFrameWorkMain game(&winInfo, 2);

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