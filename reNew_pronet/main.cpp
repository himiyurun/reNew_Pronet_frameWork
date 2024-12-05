#include <memory>

#include "Pronet.h"
#include "TLSFmemory.h"
#include "Timer.h"
#include "MemoryAllocaterTester.h"

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

int main() {

	libInit();

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

	Object* obj2 = new_class<Object>(4);
	delete_class<Object>(obj2);

	try {
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}