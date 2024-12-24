#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
	, object(nullptr)
	, shader(nullptr)
	, objPool(4)
	, shdPool(4)
	, winParamUbo("window")
{
	
}

PronetManager::~PronetManager()
{
	objPool.push(&object);
	shdPool.push(&shader);
}

void PronetManager::process()
{
	shader[0].use();
	winParamUbo.bind();

	winParamUbo.Update(&param, 1);

	object[0].draw();
}
