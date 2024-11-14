#include "readtxt.h"

readtxt::readtxt(const char* name)
	: file(name, std::ios::binary)
	, name(name)
{
	std::cout << "file read " << name << std::endl;

	if (!file.is_open()) {
		file.close();
		throw std::runtime_error("Can't Open file");
	}

	file.seekg(0L, std::ios::end);
	uint32_t bufsize(static_cast<uint32_t>(file.tellg()));
	file.seekg(0L, std::ios::beg);

	src.resize(bufsize + 1);
	file.read(src.data(), bufsize);
	src[bufsize] = '\0';

	if (file.fail()) {
		file.close();
		throw std::runtime_error("Can't Read file");
	}

	file.close();
}

readtxt::~readtxt()
{
}
