#pragma once
#include <glbs/py_db.hpp>
#include <glm/glm.hpp>

#include "Uniform.h"
#include "pronet_memory.h"
#include "Shader.h"
#include "Object.h"
#include "Texture.h"

class BulletObject
{
private:
	data_base::BULLET_FUNC_MANAGER* ptr_;
	size_t script_;

	pronet::poolObject_shared_ptr<Shader, 6> shader_;
	pronet::poolObject_shared_ptr<Object, 6> object_;
	pronet::poolObject_shared_ptr<Texture, 6> texture_;
	
	py::object generate_;
	py::object update_;

	clock_t lastClock;
public:

	BulletObject(const char* _src = nullptr
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader = pronet::poolObject_shared_ptr<Shader, 6>()
		, const pronet::poolObject_shared_ptr<Object, 6>& _object = pronet::poolObject_shared_ptr<Object, 6>()
		, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>());

	~BulletObject() {}

	void init(const char* _src
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader, const pronet::poolObject_shared_ptr<Object, 6>& _object
		, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>());

	virtual void execute(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base);
	virtual void generate(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base);
	virtual void update(float _pos[2], bool _status, data_base::BULLET_PARAM_DB* const _data_base);
	virtual void draw(data_base::BULLET_PARAM_DB* const _data_base);


	void run(float _pos[2], bool _status);
	void run(glm::vec2 _pos, bool _status);

	void reset();
};

