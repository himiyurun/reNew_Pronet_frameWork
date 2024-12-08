#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
	, test(16)
{
	object = test.pop();
	shader = new_class<Shader>(4);
}

PronetManager::~PronetManager()
{
	test.push(&object);
	delete_class<Shader>(shader);
}

void PronetManager::process() const
{
	shader[0].use();

	bindWindowPram(glGetUniformLocation(shader[0].getProgram(), "size"), glGetUniformLocation(shader[0].getProgram(), "scale"));

	glUniform2fv(glGetUniformLocation(shader[0].getProgram(), "location"), 1, nowPosition);

	object->draw();
}
