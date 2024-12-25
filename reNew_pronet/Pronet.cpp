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
	objPool.back(&object);
	shdPool.back(&shader);
}

void PronetManager::load()
{
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo info = file_reader.getLoadFile(0, &vertsPool, &indexPool);
	InitShader(info.shaders[0].vsrc.c_str(), info.shaders[0].fsrc.c_str());
	InitObj(&info.objs[0], GL_TRUE);
}

void PronetManager::process()
{
	shader[0].use();
	winParamUbo.bind();

	winParamUbo.Update(&param, 1);

	object[0].draw();
}
