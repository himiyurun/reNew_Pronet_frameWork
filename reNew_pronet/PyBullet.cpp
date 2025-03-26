#include "PyBullet.h"

void PyBullet::Initialize(const char* _python_file, const PyBulletCreateInfo& _info
	, const pronet::poolObject_shared_ptr<Object, 6>& _object
	, const pronet::poolObject_shared_ptr<Shader, 6>& _shader
	, const pronet::poolObject_shared_ptr<Texture, 6>& _texture)
{	
	init(_info.info_.id_, _info.info_.coef_, _info.info_.interval_, _info.info_.rad_, _info.info_.speed_);
	object_ = _object;
	shader_ = _shader;
	texture_ = _texture;
	PythonScript itp("pysrc");
	if (!_python_file)
		throw std::runtime_error("python source name is null : PyBullet.Initialize");

	execute_ = itp[_info.exec_func_id_.c_str()];
	generate_ = itp[_info.gen_func_id_.c_str()];
	update_ = itp[_info.update_func_id_.c_str()];
}

void PyBullet::Reset()
{
	reset();
	execute_ = py::object();
	generate_ = py::object();
	update_ = py::object();

	object_.reset();
	shader_.reset();
	texture_.reset();
}

void PyBullet::execute(glm::vec2& _pos, bool _gen_stat) {
	execute(_pos, _gen_stat);
}

void PyBullet::generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets) {
	generate_(_pos, _beg_rad, _radius, _bullets);
}

void PyBullet::update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets) {
	update_(_pos, _beg_rad, _radius, _bullets);
}