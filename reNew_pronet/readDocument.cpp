#include "readDocument.h"

pronet::PronetReadObject2v::PronetReadObject2v()
	: name(nullptr)
	, data()
	, points(0)
	, vaocount(0)
	, vertexcount(0)
	, indexcount(0)
	, nowVao(0)
{
}

pronet::PronetReadObject2v::~PronetReadObject2v()
{
}

bool pronet::PronetReadObject2v::readFile(const char* name, std::unique_ptr<vertexArrayInfo[]>& info)
{
	this->name = name;
	file.open(name, std::ios::in);

	if (!file.is_open()) {
		std::string err(name);
		err += " is not found!";
		throw std::runtime_error(err);
	}

	std::cout << name << " is open" << std::endl;

	std::getline(file, data);
	if (!type_correct(data)) {
		file.clear();
		file.close();
		throw std::runtime_error("File type is wrong!");
	}

	while (!file.eof() && std::getline(file, data)) {
		if (file.fail()) {
			std::string err = name;
			err += " can't read!";
			throw std::runtime_error(err);
		}
		getFromText(data, info);
	}
	file.clear();
	
	file.close();
	if (file.fail()) {
		std::string err = name;
		err += " can't close!";
		throw std::runtime_error(err);
	}
	data.clear();

	return true;
}

bool pronet::PronetReadObject2v::type_correct(const std::string text)
{
	iss.str(text);
	iss >> buf;
	if (buf != "#pronetObject2v") {
		std::cerr << "File type Error : '" << buf << "' is undefined references!" << std::endl;
		return false;
	}
	std::cout << "type is correct" << std::endl;

	iss.clear();
	buf.clear();
	return true;
}

void pronet::PronetReadObject2v::getFromText(const std::string text, std::unique_ptr<vertexArrayInfo[]>& info)
{
	iss.str(text);
	iss >> buf;
	switch (buf[0]) {
	case 'v':
		getVerts(buf.c_str(), info);
		break;
	case 'u':
		getUv(buf.c_str(), info);
		break;
	case 'i':
		getIndex(buf.c_str(), info);
		break;
	case 's':
		getShader(buf.c_str(), info);
		break;
	case '}':
		nowVao++;
		break;
	default:
		points = 0;
		break;
	}

	iss.clear();
	buf.clear();
}

inline void pronet::PronetReadObject2v::getVerts(const char* script, std::unique_ptr<vertexArrayInfo[]> &info)
{
	if (!script)return;
	if (strcmp(script, "vp") == 0) {
		iss >> info[nowVao].verts[points].x >> info[nowVao].verts[points].y;
		points++;
	}
	else if (strcmp(script, "verts") == 0) {
		iss >> info[nowVao].vertexcount;
		info[nowVao].verts = new_type<glm::vec2>(info[nowVao].vertexcount);
		info[nowVao].uv = new_type<glm::vec2>(info[nowVao].vertexcount);
	}
	else if (strcmp(script, "vArray") == 0){
		iss >> vaocount;
		info = std::make_unique<vertexArrayInfo[]>(vaocount);
	}
}

inline void pronet::PronetReadObject2v::getIndex(const char* script, std::unique_ptr<vertexArrayInfo[]> &info)
{
	if (!script)return;
	if (strcmp(script, "index") == 0) {
		for (int i = 0; i < info[nowVao].indexcount; i++) {
			iss >> info[nowVao].index[i];
		}
	}
	else if (strcmp(script, "indices") == 0) {
		iss >> info[nowVao].indexcount;
		info[nowVao].index = new_type<uint32_t>(info[nowVao].indexcount);
	}
	else {

	}
}

inline void pronet::PronetReadObject2v::getUv(const char* script, std::unique_ptr<vertexArrayInfo[]>& info)
{
	if (!script)return;
	if (strcmp(script, "uvp") == 0) {
		iss >> info[nowVao].uv[points].x >> info[nowVao].uv[points].y;
		points++;
	}
}

inline void pronet::PronetReadObject2v::getShader(const char* script, std::unique_ptr<vertexArrayInfo[]>& info)
{
	if (!script)return;
	if (strcmp(script, "shader") == 0) {
		iss >> info[nowVao].shader_index;
	}
}

void pronet::printVaoInfo(vertexArrayInfo *info)
{
	std::cout << "vertexcount : " << info->vertexcount << std::endl;
	std::cout << "indexcount : " << info->indexcount << std::endl;
	std::cout << "vertex" << std::endl;
	for (int i = 0; i < info->vertexcount; i++) {
		std::cout << "x : " << info->verts[i].x << ", y : " << info->verts[i].y << std::endl;
	}
	std::cout << "uv" << std::endl;
	for (int i = 0; i < info->vertexcount; i++) {
		std::cout << "x : " << info->uv[i].x << ", y : " << info->uv[i].y << std::endl;
	}
	std::cout << "index" << std::endl;
	for (int i = 0; i < info->indexcount; i++) {
		std::cout << i << " : " << info->index[i] << std::endl;
	}
}
