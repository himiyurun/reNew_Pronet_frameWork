#include <memory>

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

uint8_t pool[284];

int main() {

	libInit();

	memset(pool, 0, sizeof(pool));

	glfw_windowCreateInfo winInfo;
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

	game.InitObj(4, rectangleVertex);

	pronet::BoundaryTagBegin* begin = pronet::createNewTag(pool, 256, true);
	char* buf = reinterpret_cast<char*>(begin) + pronet::begSize;
	buf[256] = 'c';
	std::cout << buf[256] << std::endl;
	pronet::deleteTag(pool);

	try {
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}