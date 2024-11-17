#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
{
}

PronetManager::~PronetManager()
{
}

void PronetManager::process() const
{
	shader.use();

	object.draw();
}
