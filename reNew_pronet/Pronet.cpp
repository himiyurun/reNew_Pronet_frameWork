#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
	, object(nullptr)
	, shader(nullptr)
	, test(4)
	, testshader(4)
	, winParamUbo("window")
{
	
}

PronetManager::~PronetManager()
{
	shader->Reset();
	testshader.push(&shader);
	object->Reset();
	test.push(&object);
}

void PronetManager::process()
{
	shader->use();
	winParamUbo.bind();

	winParamUbo.Update(&param, 1);

	object->draw();
}
