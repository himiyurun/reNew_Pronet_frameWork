#include "PyBullet.h"

void PyBullet::Initialize(const char* _python_file, const PyBulletCreateInfo& _info
	, const pronet::poolObject_shared_ptr<Object, 6>& _object
	, const pronet::poolObject_shared_ptr<Shader, 6>& _shader
	, const pronet::poolObject_shared_ptr<Texture, 6>& _texture
	, const char* _dir)
{	
	init(_info.info_.id_, _info.info_.coef_, _info.info_.interval_, _info.info_.rad_, _info.info_.speed_);
	object_ = _object;
	shader_ = _shader;
	texture_ = _texture;
	if(_dir)
		py_script_.setDir(_dir);
	if (!_python_file)
		throw std::runtime_error("python source name is null : PyBullet.Initialize");
	py_script_.execute_file(_python_file);
	execute_ = py_script_[_info.exec_func_id_.c_str()];
	generate_ = py_script_[_info.gen_func_id_.c_str()];
	update_ = py_script_[_info.update_func_id_.c_str()];
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

void PyBullet::run(glm::vec2& _pos, bool _gen_stat)
{
	execute(_pos, _gen_stat);
	std::cout << _pos.x << ", " << _pos.y << std::endl;
	shader_->use();
	texture_->exe(shader_->getTextureUnitID());
	for (auto& a : bullet_array_) {
		pronet::updateUniformBlock(GAME_BULLET_DATA_PARAMETER, &a.param);
		for (auto& b : a.bullets_) {
			pronet::updateUniformBlock(GAME_BULLET_PARAMETER, &b);
			object_->draw();
		}
	}
}

void PyBullet::execute(glm::vec2& _pos, bool _gen_stat) {
	execute_(_pos.x, _pos.y, _gen_stat);
}

void PyBullet::generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets) {
	generate_(_pos, _beg_rad, _radius, _bullets);
}

void PyBullet::update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets) {
	update_(_pos, _beg_rad, _radius, _bullets);
}