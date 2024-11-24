#include <memory>

#include "Pronet.h"
#include "TLSFmemory.h"

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

	try {
		pronet::TLSFmemory tlsf(10, 4);
		
		char* test = static_cast<char*>(tlsf.allocate(256));
		char* test2 = static_cast<char*>(tlsf.allocate(256));
		char* test3 = static_cast<char*>(tlsf.allocate(256));
		pronet::BoundaryTagBegin* beg = reinterpret_cast<pronet::BoundaryTagBegin*>(test - pronet::begSize);
		pronet::BoundaryTagEnd* end = beg->endTag();
		std::cout << "size : " << beg->bufSize() << ", " << end->size << std::endl;
		std::cout << "diraddres : " << reinterpret_cast<uint64_t>(end) - reinterpret_cast<uint64_t>(beg) << std::endl;

		tlsf.deallocate(test);
		tlsf.printFreelistStatus();

		tlsf.deallocate(test2);

		tlsf.printFreelistStatus();

		tlsf.deallocate(test3);

		tlsf.printFreelistStatus();
		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	/*
	pronet::BoundaryTagBegin* begin = pronet::createNewTag(pool, 256, true);
	char* buf = reinterpret_cast<char*>(begin) + pronet::begSize;
	buf[256] = 'c';
	std::cout << buf[256] << std::endl;
	pronet::BoundaryTagBegin* rbegin = begin->split(128);
	rbegin->setUsed(true);
	char* rbuf = reinterpret_cast<char*>(begin) + pronet::begSize;
	for (int i = 0; i < rbegin->bufSize(); i++) {
		rbuf[i] = 'c';
	}
	for (int i = 0; i < rbegin->bufSize(); i++) {
		std::cout<<rbuf[i]<<" ";
	}
	std::cout <<rbegin->bufSize()<< std::endl;
	rbegin->setUsed(false);
	begin->marge();
	pronet::BoundaryTagEnd* end = begin->endTag();
	std::cout << "begin size : " << begin->bufSize() << ", end size : " << end->size << std::endl;
	pronet::deleteTag(buf);
	//pronet::deleteTag(rbuf);
	*/

	try {
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}