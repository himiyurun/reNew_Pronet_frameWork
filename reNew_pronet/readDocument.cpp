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

bool pronet::PronetReadObject2v::readFile(const char* name, std::unique_ptr<ObjectInfo2v[]>& info)
{
	this->name = name;
	file.open(name, std::ios::in);

	fileclose("is not found!");

	std::cout << name << " is open" << std::endl;

	std::getline(file, data);
	if (!type_correct(data)) {
		fileclose("File type is wrong!");
	}

	while (!file.eof() && std::getline(file, data)) {
		fileclose("Can't read");
		getFromText(data, info);
	}

	file.clear();
	file.close();
	fileclose("Can't Close!");
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

void pronet::PronetReadObject2v::getFromText(const std::string text, std::unique_ptr<ObjectInfo2v[]>& info)
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

inline void pronet::PronetReadObject2v::getVerts(const char* script, std::unique_ptr<ObjectInfo2v[]>& info)
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
		info = std::make_unique<ObjectInfo2v[]>(vaocount);
	}
}

inline void pronet::PronetReadObject2v::getIndex(const char* script, std::unique_ptr<ObjectInfo2v[]>& info)
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

inline void pronet::PronetReadObject2v::getUv(const char* script, std::unique_ptr<ObjectInfo2v[]>& info)
{
	if (!script)return;
	if (strcmp(script, "uvp") == 0) {
		iss >> info[nowVao].uv[points].x >> info[nowVao].uv[points].y;
		points++;
	}
}

inline void pronet::PronetReadObject2v::getShader(const char* script, std::unique_ptr<ObjectInfo2v[]>& info)
{
	if (!script)return;
	if (strcmp(script, "shader") == 0) {
		iss >> info[nowVao].shader_index;
	}
}

void pronet::PronetReadObject2v::fileclose(const char* log)
{
	if (file.fail()) {
		std::string err = name + ' ';
		err += log;
		file.close();
		throw std::runtime_error(err);
	}
}

void pronet::printVaoInfo(ObjectInfo2v* info)
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

pronet::readShaderMake::readShaderMake()
	: name(nullptr)
	, points(0)
	, size(0)
{
}

pronet::readShaderMake::~readShaderMake()
{
}

void pronet::readShaderMake::readFile(const char* name, std::unique_ptr<ShaderMakeInfo[]>& info)
{
	info.reset();
	points = 0;
	size = 0;

	file.open(name, std::ios::in);
	if (!file.is_open()) {
		thMsg("Can't Open");
	}
	else {
		std::cout << "file open " << name << " is correct" << std::endl;
	}

	std::getline(file, src);
	if (!type_correct(src.c_str())) {
		thMsg("File type may be wrong!");
	}
	else {
		std::cout << "type is correct" << std::endl;
	}

	while (!file.eof() && std::getline(file, src)) {
		iss.clear();
		script.clear();
		iss.str(src);
		iss >> script;
		std::cout << "script : " << script << std::endl;
		std::cout << "src : " << src << std::endl;
		switch (script[0]) {
		case 'V':
			scriptFunc("Vertex", [this, &info]() {
				if (info.get() == nullptr) thMsg("Shader Count is out of lange");
				std::cout << "Vertex" << std::endl;
				iss >> info[points].vsrc;
				});
			break;
		case 'F':
			scriptFunc("Fragment", [this, &info]() {
				if (info.get() == nullptr) thMsg("Shader Count is out of lange");
				std::cout << "Fragment" << std::endl;
				iss >> info[points].fsrc;
				});
			break;
		case 'u':
			break;
		case '{':
			break;
		case '}':
			points++;
			if (points >= size) {
				thMsg("Shader count is out of lange!");
			}
			break;
		case 'S':
			scriptFunc("Shaders", [this, &info]() {
				iss >> size;
				info = std::make_unique<ShaderMakeInfo[]>(size);
				std::cout << "Found Shaders" << std::endl;
				});
			break;
		default:
			break;
		}
	}

	clear();
}

inline bool pronet::readShaderMake::type_correct(const char* script)
{
	if (src != "#Pronet Shader Make"){
		clear();
		thMsg("file_type may be Wrong!");
		return false;
	}
	return true;
}

inline void pronet::readShaderMake::fileError(const char* MSG)
{
	if (file.fail()) {
		clear();
		thMsg(MSG);
	}
}

inline void pronet::readShaderMake::thMsg(const char* msg) const
{
	std::string err(name);
	err += ' ' + msg;
	throw std::runtime_error(err);
}

inline void pronet::readShaderMake::scriptFunc(const char* text, std::function<void()> func)
{
	if (std::strcmp(text, script.c_str()) == 0) {
		func();
	}
}

void pronet::readShaderMake::clear()
{
	file.close();
	fileError("Can't Close");
	file.clear();
	iss.clear();
	src.clear();
	script.clear();
}

pronet::PronetReadLoadFileList::PronetReadLoadFileList(const char* name)
	: name(name)
	, current(0)
	, points(0)
	, chanckSize(0)
	, shaderSize(0)
	, objectSize(0)
{
}

pronet::PronetReadLoadFileList::~PronetReadLoadFileList()
{
}

PronetLoadChanckInfo pronet::PronetReadLoadFileList::getLoadFile()
{
	return PronetLoadChanckInfo();
}

inline bool pronet::PronetReadLoadFileList::type_correct(const char* script)
{
	return false;
}

inline void pronet::PronetReadLoadFileList::fileError(const char* MSG)
{
}

inline void pronet::PronetReadLoadFileList::thMsg(const char* msg) const
{
}

inline void pronet::PronetReadLoadFileList::scriptFunc(const char* text, std::function<void()> func)
{
}

void pronet::PronetReadLoadFileList::clear()
{
	file.close();
	file.clear();
	fileError("Can't Close");
	line.clear();
	script.clear();
	iss.clear();
}
