#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, const char* loadfilelist_name)
	: glfw_Window(windowInfo)
	, file_reader(loadfilelist_name, &dimentionSize)
	, objPool()
	, shdPool()
	, vertsPool(64)
	, indexPool(32)
	, winParamUbo("window")
{
	object = objPool.get(1);
	shader = shdPool.get(1);
}

PronetManager::~PronetManager()
{
	str.reset();
	objPool.back(&object);
	shdPool.back(&shader);
}

void PronetManager::load(Structure2vParamCreateInfo* strInfo)
{
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo info = file_reader.getLoadFile(0, &vertsPool, &indexPool);
	InitShader(info.shaders[0].vsrc.c_str(), info.shaders[0].fsrc.c_str());
	InitObj();
	initStr(strInfo, &info.objs[0], 1, 1);
}

void PronetManager::initStr(Structure2vParamCreateInfo* strInfo, ObjectInfo2v* objInfo, uint32_t shd_index, uint32_t tex_index)
{
	str.update(strInfo, objInfo, shd_index, tex_index);
}

void PronetManager::process()
{
	shader[str.shader()].use();
	winParamUbo.bind();

	winParamUbo.Update(&param, 1);

	str.draw();
}
