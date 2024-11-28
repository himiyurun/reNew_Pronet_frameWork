#include <memory>

#include "Pronet.h"
#include "TLSFmemory.h"

void libInit() {
	if (!glfwInit()) {
		throw std::runtime_error("Can't Initlize GLFW!");
	}
}

void runMemoryAllocater(uint32_t maxsize, uint32_t minsize, time_t testTime, pronet::TLSFmemory* tlsf) {
	time_t nowtime(time(nullptr));
	time_t lasttime(time(nullptr));

	const uint8_t maxAllocation(4);

	uint32_t size(0);
	uint32_t allocateCount(0);
	uint32_t deallocateCount(0);
	std::vector<char*> ptr;
	while (nowtime - lasttime <= testTime) {

		allocateCount = rand() % maxAllocation;
		deallocateCount = rand() % maxAllocation;

		for (int i = 0; i < allocateCount; i++) {
			size = minsize + rand() % (maxsize - minsize);
			char* alocPtr = static_cast<char*>(tlsf->allocate(size));
			std::cout << "Allocate Size : " << size << std::endl;
			if (alocPtr) {
				ptr.emplace_back(alocPtr);
				/*
				char* buf = ptr.back();
				for (int j = 0; j < size; j++) {
					if (j % 32 == 0)
						buf[j] = 'T';
					else if (j % 8 == 0)
						buf[j] = 'S';
					else
						buf[j] = 'c';
				}
				std::cout << &ptr.back()[0] << std::endl;
				*/
			}
#ifdef _DEBUG
			else
				std::cout << std::endl << "May be Memory Pool is FULL" << std::endl << std::endl;
#endif // _DEBUG
			tlsf->printFreelistStatus();
			std::cout << std::endl;
			tlsf->printMemoryLayout();
			std::cout << std::endl;
		}
		for (int i = 0; i < deallocateCount; i++) {
			size = minsize + rand() % (maxsize - minsize);
			if (!ptr.empty()) {
				tlsf->deallocate(ptr.front());
				ptr.erase(ptr.begin());
			}
			tlsf->printFreelistStatus();
			std::cout << std::endl;
			tlsf->printMemoryLayout();
			std::cout << std::endl;
		}

		nowtime = time(nullptr);
	}

	std::cout << "MemoryAllocateTest is Finish!!" << std::endl;
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

		runMemoryAllocater(128, 32, 10, &tlsf);
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