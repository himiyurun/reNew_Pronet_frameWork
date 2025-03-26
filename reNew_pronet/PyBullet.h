#pragma once
#include <string>
#include "PythonScript.h"
#include "BulletManager.h"

typedef struct {
	BulletCreateInfo info_;
	std::string exec_func_id_;
	std::string gen_func_id_;
	std::string update_func_id_;
}PyBulletCreateInfo;

namespace py = boost::python;
class PyBullet : public Bullet
{
	//	íeñãÇÃèàóùëSî 
	py::object execute_;
	//	íeñãÇÃê∂ê¨éûÇÃèàóù
	py::object generate_;
	//	íeñãÇÃçXêVéûÇÃèàóù
	py::object update_;

	pronet::poolObject_shared_ptr<Object, 6> object_;
	pronet::poolObject_shared_ptr<Shader, 6> shader_;
	pronet::poolObject_shared_ptr<Texture, 6> texture_;
public:
	PyBullet(const char* _python_file = nullptr, const PyBulletCreateInfo& _info = PyBulletCreateInfo()
		, const pronet::poolObject_shared_ptr<Object, 6>& _object = pronet::poolObject_shared_ptr<Object, 6>()
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader = pronet::poolObject_shared_ptr<Shader, 6>()
		, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>())
		: Bullet(_info.info_.id_, _info.info_.coef_, _info.info_.interval_, _info.info_.rad_, _info.info_.speed_)
		, object_(_object), shader_(_shader), texture_(_texture)
	{
		PythonScript itp("pysrc");
		itp.execute_file(_python_file);
		if (_python_file) {
			execute_ = itp[_info.exec_func_id_.c_str()];
			generate_ = itp[_info.gen_func_id_.c_str()];
			update_ = itp[_info.update_func_id_.c_str()];
		}
	}

	~PyBullet() {}

	void Initialize(const char* _python_file, const PyBulletCreateInfo& _info
		, const pronet::poolObject_shared_ptr<Object, 6>& _object
		, const pronet::poolObject_shared_ptr<Shader, 6>& _shader
		, const pronet::poolObject_shared_ptr<Texture, 6>& _texture = pronet::poolObject_shared_ptr<Texture, 6>());

	void Reset();

	virtual void execute(glm::vec2& _pos, bool _gen_stat);

	virtual void generate(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets);

	virtual void update(glm::vec2& _pos, float& _beg_rad, float& _radius, std::vector<BulletParam, pnTlsfInsertSTLtype<BulletParam>>& _bullets);
};

