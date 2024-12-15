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

	pronet::PronetReadObject2v objfile;
	pronet::readShaderMake shaderfile;
	std::unique_ptr<ObjectInfo2v[]> hoge;
	std::unique_ptr<ShaderMakeInfo[]> shader;

	try {
		objfile.readFile("test.str2v", hoge);
		shaderfile.readFile("sample.pnsm", shader);
		game.InitShader(shader[0].vsrc.c_str(), shader[0].fsrc.c_str());
		game.InitObj(&hoge[0], GL_TRUE);
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	hoge[0].delete_type<glm::vec2>(hoge[0].verts);
	hoge[0].delete_type<uint32_t>(hoge[0].index);
	hoge[0].delete_type<glm::vec2>(hoge[0].uv);
	return EXIT_SUCCESS;
}