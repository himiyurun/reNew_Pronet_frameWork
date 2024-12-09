#include "Pronet.h"

PronetManager::PronetManager(glfw_windowCreateInfo* windowInfo, GLint dimentionSize)
	: glfw_Window(windowInfo)
	, dimentionSize(dimentionSize)
	, object(nullptr)
	, shader(nullptr)
	, test(4)
	, testshader(4)
{
}

PronetManager::~PronetManager()
{
	shader->Reset();
	testshader.push(&shader);
	object->Reset();
	test.push(&object);
}

void PronetManager::process() const
{
	shader->use();

	bindWindowPram(glGetUniformLocation(shader[0].getProgram(), "size"), glGetUniformLocation(shader[0].getProgram(), "scale"));

	glUniform2fv(glGetUniformLocation(shader[0].getProgram(), "location"), 1, nowPosition);

	object->draw();
}
