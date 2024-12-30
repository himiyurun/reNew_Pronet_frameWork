#include "readDocument.h"

/*	read Object 2v impl
* 
* 
*/

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

bool pronet::PronetReadObject2v::read_pnObject2v(const char* name, ObjectInfo2v *info, pronet::ObjectPool_Array<glm::vec2> *vertsPool, pronet::ObjectPool_Array<uint32_t> *indexPool)
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
		getFromText(data, info, vertsPool, indexPool);
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

void pronet::PronetReadObject2v::getFromText(const std::string text, ObjectInfo2v* info, pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool)
{
	iss.str(text);
	iss >> buf;
	switch (buf[0]) {
	case 'v':
		getVerts(buf.c_str(), info, vertsPool);
		break;
	case 'u':
		getUv(buf.c_str(), info);
		break;
	case 'i':
		getIndex(buf.c_str(), info, indexPool);
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

inline void pronet::PronetReadObject2v::getVerts(const char* script, ObjectInfo2v* info, pronet::ObjectPool_Array<glm::vec2> *vertsPool)
{
	if (!script)return;
	if (strcmp(script, "vp") == 0) {
		iss >> info->verts[points].x >> info[nowVao].verts[points].y;
		points++;
	}
	else if (strcmp(script, "verts") == 0) {
		iss >> info->vertexcount;
		info->verts = vertsPool->get(info->vertexcount);
		info->uv = vertsPool->get(info->vertexcount);
	}
	else if (strcmp(script, "vArray") == 0){
		iss >> vaocount;
		//info = std::make_unique<ObjectInfo2v[]>(vaocount);
	}
}

inline void pronet::PronetReadObject2v::getIndex(const char* script, ObjectInfo2v* info, pronet::ObjectPool_Array<uint32_t>* indexPool)
{
	if (!script)return;
	if (strcmp(script, "index") == 0) {
		for (int i = 0; i < info[nowVao].indexcount; i++) {
			iss >> info->index[i];
		}
	}
	else if (strcmp(script, "indices") == 0) {
		iss >> info->indexcount;
		info->index = indexPool->get(info->indexcount);;
	}
	else {

	}
}

inline void pronet::PronetReadObject2v::getUv(const char* script, ObjectInfo2v* info)
{
	if (!script)return;
	if (strcmp(script, "uvp") == 0) {
		iss >> info[nowVao].uv[points].x >> info[nowVao].uv[points].y;
		points++;
	}
}

inline void pronet::PronetReadObject2v::getShader(const char* script, ObjectInfo2v* info)
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

//	read object 2v impl end.

/*	read shader make impl
* 
* 
*/

pronet::readShaderMake::readShaderMake()
	: name(nullptr)
	, points(0)
	, size(0)
{
}

pronet::readShaderMake::~readShaderMake()
{
}

void pronet::readShaderMake::read_ShaderMake(
	const char* name, 
	pronet::poolArray_unique_ptr<ShaderMakeInfo>* info, 
	pronet::ObjectPool_Array<ShaderMakeInfo>* pool)
{
	if (!info)throw std::bad_alloc();
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
		switch (script[0]) {
		case 'V':
			//	バーテックスシェーダーの読み込み
			scriptFunc("Vertex", [this, info]() {
				if (!info) thMsg("Shader Count is out of lange");
				std::cout << "Vertex" << std::endl;
				iss >> info->operator[](points).vsrc;
				});
			break;
		case 'F':
			//	フラグメントシェーダーの読み込み
			scriptFunc("Fragment", [this, info]() {
				if (!info) thMsg("Shader Count is out of lange");
				std::cout << "Fragment" << std::endl;
				iss >> info->operator[](points).fsrc;
				});
			break;
		case 'u':
			break;
		case '{':
			break;
		case '}':
			//	1組のシェーダの読み込みの終了
			points++;
			if (points > size) {
				thMsg("Shader count is out of lange!");
			}
			break;
		case 'S':
			//	シェーダーのサイズを取得
			scriptFunc("Shaders", [this, info, pool]() {
				iss >> size;
				info->realloc(static_cast<size_t>(size), pool);
				std::cout << "Found Shaders " << info->operator->()->size << std::endl;
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

//	read shader make impl end.

/*	read load file list impl
* 
* 
*/

pronet::PronetReadLoadFileList::PronetReadLoadFileList(const char* name, int* dimentionSize)
	: name(name)
	, current(0)
	, points(0)
	, geometory(0)
	, chanckSize(0)
	, shaderSize(0)
	, objectSize(0)
{
	file.open(name, std::ios::in);
	if (!file.is_open()) {
		thMsg("Can't Open!");
	}

	std::getline(file, line);
	if (!type_correct("#Pronet Load File List")) {
		clear();
		thMsg("file type is not correct!!");
	}
	std::getline(file, line);
	if (strcmp(line.c_str(), "#Pronet_2D") == 0) {
		*dimentionSize = 2;
	}
	else if (strcmp(line.c_str(), "#Pronet_3D") == 0) {
		*dimentionSize = 3;
	}
	else {
		clear();
		thMsg("Dimention size undefined!!");
	}

	std::cout << "Dimentionsize : " << *dimentionSize << std::endl;

	while (std::getline(file, line) && chanckSize == 0) {
		iss.clear();
		script.clear();
		iss.str(line);
		iss >> script;
		scriptFunc("chanckCount", [this] {
			iss >> this->chanckSize;
			});
	}

	geometory = static_cast<uint8_t>(file.tellg());
}

pronet::PronetReadLoadFileList::~PronetReadLoadFileList()
{
	clear();
}

pronet::PronetReadLoadFileList::PronetLoadChanckInfo pronet::PronetReadLoadFileList::get_pnLCI(
	uint32_t chanck_Index, pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool)
{
	if (chanck_Index >= chanckSize) {
		clear();
		thMsg("chanck_Index out of lange!!!");
	}
	file.seekg(geometory, std::ios::beg);

	points = 0;
	while (!file.eof() && chanck_Index >= points) {
		flash();
		std::getline(file, line);
		iss.str(line);
		iss >> script;
		scriptFunc("Chanck", [this](){
			points++;
			});
	}
	std::cout << "finish" << std::endl;

	return getParam(vertsPool, indexPool);
}

inline pronet::PronetReadLoadFileList::PronetLoadChanckInfo pronet::PronetReadLoadFileList::getParam(
	pronet::ObjectPool_Array<glm::vec2> *objPool, pronet::ObjectPool_Array<uint32_t> *shdPool)
{
	PronetLoadChanckInfo info;
	uint32_t objSize, shaderSize;
	uint32_t objCurrent = 0, shaderCurrent = 0;
	while (strcmp(script.c_str(), "}") != 0 && std::getline(file,line)) {
		flash();
		iss.str(line);
		iss >> script;
		switch (script[0]) {
		case 'O':
			scriptFunc("Objs", [this, &info, &objSize] {
				if (!info.objs) {
					iss >> objSize;
					info.objs.realloc(objSize, &_obj_infop);
					std::cout << "objsize : " << info.objs->size << std::endl;
				}
#ifdef _DEBUG
				else {
					std::cerr << "Msg : Shader count is already specified!" << std::endl;
				}
#endif
				});
			scriptFunc("Object", [this, &info, &objCurrent, objSize, objPool, shdPool] {
#ifdef _DEBUG
				if (objSize == 0) {
					std::cerr << "MSG : Shader size may be wrong!" << std::endl;
				}
#endif
				for (int i = 0; i < objSize; i++) {
					std::getline(file, line);
					iss.str(line);
					iss >> script;
					read_pnObject2v(script.c_str(),&info.objs[i], objPool, shdPool);
				}
				std::getline(file, line);
				if (strcmp(line.c_str(), "}") == 0) {
					std::cout << "Object list read Finished properly" << std::endl;
				}
				});
			break;
		case 'S':
			scriptFunc("Shaders", [this, &info, &shaderSize] {
				if (!info.shaders) {
					iss >> shaderSize;
					info.shaders.realloc(shaderSize, &_shd_infop);
					std::cout << "shader size : " << info.shaders->size << std::endl;
				}
#ifdef _DEBUG
				else {
					std::cerr << "Msg : Shader count is already specified!" << std::endl;
				}
#endif
				});
			scriptFunc("Shader", [this, &info, &objCurrent, shaderSize, objSize] {
#ifdef _DEBUG
				if (objSize == 0) {
					std::cerr << "MSG : Shader size may be wrong!" << std::endl;
				}
#endif
				for (int i = 0; i < shaderSize; i++) {
					std::getline(file, line);
					iss.str(line);
					iss >> script;
					read_ShaderMake(script.c_str(), &info.shaders, &_shd_infop);
					std::cout << "read finish " << script << std::endl;
				}
				std::getline(file, line);
				if (strcmp(line.c_str(), "}") == 0) {
					std::cout << "Shader list read Finished properly" << std::endl;
				}
				});
			break;
		case 'T':
			break;
		default:
			break;
		}
	}
	iss.clear();
	script.clear();
	line.clear();

	return info;
}

inline bool pronet::PronetReadLoadFileList::type_correct(const char* script)
{
	if (line != script) {
		clear();
		thMsg("file_type may be Wrong!");
		return false;
	}
	return true;
}

inline void pronet::PronetReadLoadFileList::fileError(const char* MSG)
{
	if (file.fail()) {
		clear();
		thMsg(MSG);
	}
}

inline void pronet::PronetReadLoadFileList::thMsg(const char* msg) const
{
	std::string err(name);
	err += ' ' + msg;
	throw std::runtime_error(err);
}

inline void pronet::PronetReadLoadFileList::scriptFunc(const char* text, std::function<void()> func)
{
	if (strcmp(script.c_str(), text) == 0) {
		func();
	}
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

void pronet::PronetReadLoadFileList::flash()
{
	iss.clear();
	script.clear();
}

//	read load file list impl end.