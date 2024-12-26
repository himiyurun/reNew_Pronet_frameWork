#include "Structure.h"

Structure2v::Structure2v()
	: buffer_object(nullptr)
	, shader_index(0)
	, texture_index(0)
	, col_pos{ 0.0f, 0.0f }
	, col_size{ 0.0f, 0.0f }
{
}

Structure2v::~Structure2v()
{
}

void Structure2v::init(Object* new_object)
{
	if (new_object == nullptr)
		throw std::bad_alloc();
	buffer_object = new_object;
}

void Structure2v::update(Structure2vParamCreateInfo* strInfo, ObjectInfo2v* objInfo, uint32_t shd_index, uint32_t tex_index)
{
	if (buffer_object == nullptr)
		throw std::runtime_error("Structure.init must excute before excute Structure.update!");
	if (shd_index == 0)
		throw std::runtime_error("shd_index is 0 Structure.update");
	if (tex_index == 0)
		throw std::runtime_error("tex_index is 0 Structure.update");

	//	パラメーターの代入
	param.location[0] = strInfo->location[0]; param.location[1] = strInfo->location[1];
	param.rotate = strInfo->rotate;
	col_pos[0] = strInfo->col_pos[0]; col_pos[1] = strInfo->col_pos[1];
	col_size[0] = strInfo->col_size[0]; col_size[1] = strInfo->col_size[1];
	//	その他データの代入
	buffer_object->init(2, objInfo, GL_TRUE);
	shader_index = shd_index;
	texture_index = tex_index;
}

void Structure2v::reset()
{
	clear();
	buffer_object = nullptr;
}

void Structure2v::clear()
{
	//	パラメーターのリセット
	param.location[0] = 0.0f; param.location[1] = 0.0f;
	param.rotate = 0.0f;
	col_pos[0] = 0.0f; col_pos[1] = 0.0f;
	col_size[0] = 0.0f; col_size[1] = 0.0f;
	//	その他データのリセット
	buffer_object->reset();
	shader_index = 0;
	texture_index = 0;
}

void Structure2v::draw() const
{
	buffer_object->draw();
}
