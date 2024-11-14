#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

class readtxt
{
	std::vector<char> src;

	std::ifstream file;

	const char* name;

public:
	readtxt(const char* name);

	~readtxt();

	[[nodiscard]] const char* c_ptr() const { return src.data(); }

	[[nodiscard]] int getsize() const { return src.size(); }

	[[nodiscard]] bool filestat() const { return file.fail(); }
};

