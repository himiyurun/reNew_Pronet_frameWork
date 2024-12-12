#include <memory>

#include "Pronet.h"
#include "readDocument.h"

void libInit() {
	if (!glfwInit()) {
		throw std::runtime_error("Can't Initlize GLFW!");
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
	PronetManager game(&winInfo, 2);

	game.InitShader("vertex_shader.glslc", "fragment_shader.glslc");

	ObjectInfo2v objInfo{};

	pronet::PronetReadObject2v objfile;
	std::unique_ptr<pronet::vertexArrayInfo[]> hoge;

	try {
		objfile.readFile("test.str2v", hoge);
		objInfo.vertexcount = hoge[0].vertexcount;
		objInfo.vertex = hoge[0].verts;
		objInfo.indexcount = hoge[0].indexcount;
		objInfo.index = hoge[0].index;
		game.InitObj(&objInfo, GL_TRUE);
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}