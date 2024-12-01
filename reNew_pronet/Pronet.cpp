#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
	, tlsf(10, 4)
{
	object = static_cast<Object*>(tlsf.allocate(sizeof(Object) * 8));
	shader = static_cast<Shader*>(tlsf.allocate(sizeof(Shader) * 4));
}

PronetManager::~PronetManager()
{
	tlsf.deallocate(object);
	tlsf.deallocate(shader);
}

void PronetManager::process() const
{
	shader[0].use();

	object[0].draw();
}
