#define GLFW_INCLUDE_NONE

#include "Shader.h"
#include "readtxt.h"
#include "glfw_Window.h"

void PronetInit() {
	if (!glfwInit()) {
		throw std::runtime_error("Can't Initlize GLFW!");
	}
}

int main() {

	PronetInit();

	glfw_windowCreateInfo winInfo;
	winInfo.width = 640;
	winInfo.height = 480;
	winInfo.title = "syu-thingumoruka-";
	winInfo.monitor = nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfw_Window Window(winInfo);

	Shader shader("vertex_shader.glslc", "fragment_shader.glslc");

	try {
		shader.use();
		Window.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}