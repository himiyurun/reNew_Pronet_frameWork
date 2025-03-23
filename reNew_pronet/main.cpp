#define PRONET_2D
#define _POOL_DEBUG
#define STRUCTURE_POOL_BIT_MAP_LEVEL	(6)
#define CHANCK_LOAD_SIZE	(1)

#include "Pronet.h"

void libInit() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!glfwInit()) {
		std::cerr << "Error : Can't Initlize GLFW" << std::endl;
		exit(1);
	}
}

class ins {
	int itt;
	float f;
public:
	ins(int _i = 0, float _f = 0) {
		itt = _i;
		f = _f;
	}
	void init(int _i, float _f) {
		itt = _i;
		f = _f;
	}
	void reset() {
		itt = 0;
		f = 0;
	}

	int geti() const { return itt; }
	float getf() const { return f; }
};

class test {
	pronet::poolObject_shared_ptr<ins, 6> itr;
	pronet::poolObject_shared_ptr<ins, 6> itr2;
	public:
	test(const pronet::poolObject_shared_ptr<ins, 6>& _itr = nullptr, const pronet::poolObject_shared_ptr<ins, 6>& _itr2 = nullptr) 
		{}

	void init(const pronet::poolObject_shared_ptr<ins, 6>& _itr, const pronet::poolObject_shared_ptr<ins, 6>& _itr2) {
		itr = _itr;
		itr2 = _itr2;
	}

	void print_bit(){
		uint8_t* ptr = reinterpret_cast<uint8_t*>(this);
		for (size_t i = 0; i < sizeof(test); i++) {
			for (size_t i = 0; i < UNSIGNED_INT_8; i++) {
				if (*ptr & (static_cast<uint64_t>(1) << i))
					std::cout << "1";
				else
					std::cout << "0";
			}
			ptr++;
		}
		std::cout << '\n';
	}
	void reset() {
		itr.reset();
		itr2.reset();
	}
	int get() const { return itr2->geti(); }
	float getf() const { return itr2->getf(); }
	int get2() const { return itr->geti(); }
	float getf2() const { return itr->getf(); }
};

int main() {
	libInit();
	
	glfw_windowCreateInfo winInfo = {};
	winInfo.width = 640;
	winInfo.height = 480;
	winInfo.title = "test_game";
	winInfo.monitor = nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	PronetManager<6, 6> game(&winInfo, "LoadFileList.fi");
	
	/*
	pronet::ObjectPool<test, 6> hoge(32);
	pronet::ObjectPool<ins, 6> hoge2;
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<ins, 6>> spa(3);
	for (auto& a : spa) {
		a.realloc(&hoge2);
		a->init(324, 123.f);
	}
	std::shared_ptr<int> tester(new int(16));
	pronet::poolObject_shared_ptr<test, 6> hogep(&hoge);
	pronet::poolObject_shared_ptr<test, 6> hogep2(&hoge);
	hoge.resize(96);
	hogep->print_bit();
	hogep2->print_bit();
	std::cout << "stdusecount : " << tester.use_count() << std::endl;
	std::cout << "beg" << std::endl;
	hogep->init(spa[0], spa[1]);
	hogep2->init(nullptr, nullptr);
	hogep->print_bit();
	hogep2->print_bit();
	std::cout << "beg" << std::endl;
	hogep2->init(spa[1], spa[2]);
	std::cout << "end" << std::endl;
	hogep->print_bit();
	hogep2->print_bit();
	spa.reset();
	std::cout << "usecount : " << hogep().use_count() << ", " << hogep2().use_count() << std::endl;
	std::cout << "hogep : " + std::to_string(hogep->get()) << std::endl;
	std::cout << "hogep2 : " + std::to_string(hogep2->get2()) << std::endl;
	*/

	try {
		game.load();
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error : " << e.what() << std::endl;
		std::cout << "プログラムを終了するには Enter キーを押してください" << std::endl;
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}