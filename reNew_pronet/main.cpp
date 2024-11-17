#define GLFW_INCLUDE_NONE

#include "Pronet.h"
#include "BoundaryTag.h"

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

static constexpr uint8_t pool[1024];

int main() {

	libInit();

	glfw_windowCreateInfo winInfo;
	winInfo.width = 640;
	winInfo.height = 480;
	winInfo.title = "syu-thingumoruka-";
	winInfo.monitor = nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	PronetManager game(winInfo, 2);

	game.InitShader("vertex_shader.glslc", "fragment_shader.glslc");

	game.InitObj(4, rectangleVertex);

	pronet::BoundaryTagBegin

	try {
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}