#ifndef PY_DB_HPP_
#define PY_DB_HPP_
#define DLL_EXPORTS
#define BOOST_PYTHON_STATIC_LIB

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <boost/python.hpp>

#ifdef DLL_EXPORTS
#define PY_BULLET_API	__declspec(dllexport)
#else
#define PY_BULLET_API	__declspec(dllimport)
#endif // DLL_EXPORTS

#ifdef GLBS_PYTHON_DB_STATIC_LIB
#else

namespace data_base {
	
	PY_BULLET_API typedef struct {
		float positionX;	//	�e����X���W
		float positionY;	//	�e����Y���W
		float deg;			//	�e���̊p�x
	}BULLET_OBJECT_PARAM;

	PY_BULLET_API typedef struct {
		float positionX;		//	�e���̌��_��X���W
		float positionY;		//	�e���̌��_��Y���W
		float begin_degree;		//	�e���̎n�܂�p�x
		float radiculus;		//	���a�i�����j
	}BULLET_OBJECT_DATA;

	PY_BULLET_API typedef struct {
		BULLET_OBJECT_DATA data;					//	�e���̃f�[�^
		std::vector<BULLET_OBJECT_PARAM> bullet;	//	�e�e���̒l
	}BULLET_OBJECT_MANAGER;

	PY_BULLET_API typedef struct {
		float coef;			//	�W���i���a�j
		float area;			//	�e���̗L���Ȕ͈�
		size_t count;		//	�e���̔��˂����
		uint32_t interval;	//	�e���̔��˂���Ԋu
		float rad;			//	���˂���p�x�͈̔�
		float speed;		//	�e���̑��x
	}BULLET_PARAM_DATA;

	PY_BULLET_API typedef struct{
		BULLET_PARAM_DATA data;						//	�e���̊�{���
		std::vector<BULLET_OBJECT_MANAGER> bullets;	//	�e�e���̃f�[�^
	}BULLET_PARAM_DB;

	//	�e���̍X�V���Ɏ��s�����֐�
	//	pos : ���W�A�o�C���h���ꂽ�G�̈ʒu���n�����
	//	status : �����蔻��
	//	param_data : �e���̃f�[�^
	//	index : �e���I�u�W�F�N�g�̃C���f�b�N�X���Ԃ����
	//	obj_param : ���̒e���I�u�W�F�N�g�̃p�����[�^�[
	PY_BULLET_API typedef void(*BULLET_UPDATE_FUNC)(float _pos[2], bool _status, const BULLET_PARAM_DATA& _param_data, BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, BULLET_OBJECT_PARAM& _obj_param);

	//	�e���̐������Ɏ��s�����֐�
	//	pos : ���W�A�o�C���h���ꂽ�G�̈ʒu���n�����
	//	status : �����蔻��
	//	param_data : �e���̃f�[�^
	//	beg_pos : �e���̌��_�i�n�_�j
	//	beg_rad : �e���̎n�߂�p�x
	//	radiculous : �e���̏����̔��a
	//	obj_data : �V���ɍ쐬���ꂽ�e���I�u�W�F�N�g�̃f�[�^
	PY_BULLET_API typedef void(*BULLET_GENERATE_FUNC)(float _pos[2], bool _status, const BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, BULLET_OBJECT_DATA& _obj_data);

	//	�v���O�����̎��s���Ɏ��s�����֐�
	//	pos : ���W�A�o�C���h���ꂽ�G�̈ʒu���n�����
	//	status : �����蔻��
	//	data_base : �e���̃f�[�^
	//	upd_func : �X�V���Ɏ��s�����֐�
	//	gen_func : �������Ɏ��s�����֐�
	PY_BULLET_API typedef void(*BULLET_EXECUTE_FUNC)(float _pos[2], bool _status, size_t _script, BULLET_PARAM_DB& _data_base, BULLET_UPDATE_FUNC _upd_func, BULLET_GENERATE_FUNC _gen_func);

	PY_BULLET_API typedef struct {
		const char* id;
		BULLET_PARAM_DB data_base;
		size_t script;
		BULLET_EXECUTE_FUNC exec;
		BULLET_UPDATE_FUNC update;
		BULLET_GENERATE_FUNC generate;
	}BULLET_FUNC_MANAGER;

	extern std::vector<BULLET_FUNC_MANAGER> PARAM_DB;
	extern BULLET_FUNC_MANAGER* bind_obj;
	extern clock_t last_time;	

	//	BULLET_PARAM_DB�\���̂�����������֐�
	PY_BULLET_API BULLET_PARAM_DB initializeBulletParamDB(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);
	//	BULLET_FUNC_MANAGER�\���̂�����������֐�
	PY_BULLET_API BULLET_FUNC_MANAGER initializeBulletSystemManager(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed, size_t _script, BULLET_EXECUTE_FUNC _exec, BULLET_UPDATE_FUNC _upd, BULLET_GENERATE_FUNC _gen);
	//	�w�肵���C���f�b�N�X��BULLET_FUNC_MANGER�\���̂�����������
	PY_BULLET_API void resetBulletFunctionManager(size_t _index);

	//	�Ō�Ɏ��s�������Ԃ��擾
	PY_BULLET_API clock_t getLastClock();
	//	���Ԃ����݂̎����ɍX�V����
	PY_BULLET_API void updateLastClock();

#define glbsGetLastClock()		data_base::getLastClock()
#define glbsUpdateLastClock()	data_base::updateLastClock()
}

namespace py = boost::python;
namespace py_obj {
	PY_BULLET_API typedef struct {
		const char* bullet_id;
		py::object exec;
		py::object gen;
		py::object upd;
		float coef;
		float area;
		size_t count;
		uint32_t interval;
		float rad;
		float speed;
	}PYTHON_SETUP_INFO;

	PY_BULLET_API typedef struct {
		const char* name;
		py::object global;
		py::object local;
	}PYTHON_FUNCTION_OBJECT;

	PY_BULLET_API typedef struct {
		py::object _execute;
		py::object _generate;
		py::object _update;
	}PYTHON_BULLET_FUNCTION_OBJECT;

	extern std::vector<PYTHON_FUNCTION_OBJECT> py_func_db;
	extern std::vector<PYTHON_BULLET_FUNCTION_OBJECT> py_bulfunc_db;
	extern std::vector<PYTHON_SETUP_INFO> py_info_db;
	extern PYTHON_BULLET_FUNCTION_OBJECT* py_func_bind_obj;

	//	�e���f�[�^�̊�{���
	extern data_base::BULLET_PARAM_DATA* bulparam_bind_obj;
	//	1�񂠂���ɔ��˂��ꂽ�e�����Ƃ̊�{���
	extern data_base::BULLET_OBJECT_DATA* bulobj_data_bind_obj;
	//	�e�e���̏��
	extern data_base::BULLET_OBJECT_PARAM* bulobj_param_bind_obj;

	//	�p�C�\���̃X�N���v�g�I�u�W�F�N�g���쐬����
	//	file_name : �t�@�C����
	PY_BULLET_API size_t createPythonFunctionObject(const char* _file_name);

	//	�e���I�u�W�F�N�g������������֐������
	//	exec : ���s���Ɏ��s����֐��i���݂̓e���v���[�g�̂��炩���ߐݒ肳�ꂽ�֐������s�����j
	//	gen : �e���������Ɏ��s�����֐�
	//	upd : �e���X�V���Ɏ��s�����֐�
	//	coef : �W��
	//	count : �e���̐�����
	//	interval : �e���̔��˂���Ԋu�i���ԁj
	//	rad : �e���̔��˂���p�x�̑傫���i�x���@�j
	//	speed : �e���̑��x
	PY_BULLET_API size_t createPythonSetupInfo(py::object _exec, py::object _gen, py::object _upd, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);

	//	�������������Ƃɒe���I�u�W�F�N�g�����i�ԍ��͏��������̔ԍ��������p���j
	PY_BULLET_API void createBulletObjectByPython();

	//	�p�C�\���̃X�N���v�g�̃f�[�^�x�[�X������������
	//	file_name : �t�@�C����
	//	str : �f�[�^���������ލ\����
	PY_BULLET_API void initializePythonFunctionObject(const char* _file_name, PYTHON_FUNCTION_OBJECT& _str);

	//	�w�肵���֐����̊֐��I�u�W�F�N�g���擾����
	//	index : �p�C�\���I�u�W�F�N�g�̔ԍ�
	//	str : �����擾����\���́i�Q�Ɓj	
	PY_BULLET_API py::object getFunctionObject(size_t _script, const char* _func_name);

	//	�w�肵���p�C�\���̊֐��I�u�W�F�N�g���o�C���h����
	//	index : �p�C�\���̊֐��I�u�W�F�N�g�̃C���f�b�N�X
	PY_BULLET_API void bindPythonBulletFuncObject(size_t _bulfunc);
	PY_BULLET_API void executeBindPythonBulletFuncExecute(const data_base::BULLET_PARAM_DATA& _data, data_base::BULLET_OBJECT_DATA& _obj);
	PY_BULLET_API void executeBindPythonBulletFuncGenerate(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
	PY_BULLET_API void executeBindPythonBulletFuncUpdate(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);

	//	�w�肵���I�u�W�F�N�g���o�C���h���ăp�C�\���ł̏����Ώۂɂ���
	PY_BULLET_API void setBulletParamDataBinding(data_base::BULLET_PARAM_DATA* const _ptr);
	PY_BULLET_API void setBulletObjectDataBinding(data_base::BULLET_OBJECT_DATA* const _ptr);
	PY_BULLET_API void setBulletObjectParamBinding(data_base::BULLET_OBJECT_PARAM* const _ptr);

	//	�o�C���h�����I�u�W�F�N�g���擾����
	PY_BULLET_API data_base::BULLET_PARAM_DATA& getBulletParamDataBinding();
	PY_BULLET_API data_base::BULLET_OBJECT_DATA& getBulletObjectDataBinding();
	PY_BULLET_API data_base::BULLET_OBJECT_PARAM& getBulletObjectParamBinding();

	//	�o�C���h�����I�u�W�F�N�g�ɑ������
	PY_BULLET_API void assginBulletParamDataBinding(float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);
	PY_BULLET_API void assginBulletObjectDataBinding(float _positionX, float _positionY, float _begin_degree, float _radiculus);
	PY_BULLET_API void assginBulletObjectParamBinding(float _positionX, float _positionY, float _deg);
}

namespace bullet_system {
	namespace _template {
		//	�e���̎��s���Ɏ��s����֐��̃e���v���[�g
		PY_BULLET_API void bullet_execute_main(float _pos[2], bool _status, size_t _script, data_base::BULLET_PARAM_DB& _data_base, data_base::BULLET_UPDATE_FUNC _upd_func, data_base::BULLET_GENERATE_FUNC _gen_func);
		PY_BULLET_API void bullet_execute_python(float _pos[2], bool _status, size_t _script, data_base::BULLET_PARAM_DB& _data_base, data_base::BULLET_UPDATE_FUNC _upd_func, data_base::BULLET_GENERATE_FUNC _gen_func);

		//	�e���̐������Ɏ��s����֐��̃e���v���[�g
		PY_BULLET_API void bullet_generate_main(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
		PY_BULLET_API void bullet_generate_python(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
		
		//	�e���̍X�V���Ɏ��s����֐��̃e���v���[�g
		PY_BULLET_API void bullet_update_main(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);
		PY_BULLET_API void bullet_update_python(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);
		
#define EXECUTE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_execute_main
#define GENERATE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_generate_main
#define UPDATE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_update_main

#define EXECUTE_FUNCTION_TEMPLATE_PYTHON	bullet_system::_template::bullet_execute_python
#define GENERATE_FUNCTION_TEMPLATE_PYTHON	bullet_system::_template::bullet_generate_python
#define UPDATE_FUNCTION_TEMPLATE_PYTHON		bullet_system::_template::bullet_update_python
	}

	//	�e���̃f�[�^�����
	//	bullet_id : �e���̖��O
	//	coef : 1�x�ɔ��˂���e���̌�
	//	interval : �e���̔��˂���p�x
	//	rad : �e���̔��˂���p�x
	//	speed : �e���̑��x	
	PY_BULLET_API size_t createBulletDB(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed, size_t _script
		, data_base::BULLET_UPDATE_FUNC _upd, data_base::BULLET_GENERATE_FUNC _gen, data_base::BULLET_EXECUTE_FUNC _exec = _template::bullet_execute_main);

	//	�e�����폜����
	//	index : �e���̃C���f�b�N�X�ԍ�
	//	�� ���̊֐��͌��݁A�@�\��񋟂��Ă��܂���!!
	PY_BULLET_API void deleteBulletDB(size_t _bullet);

	//	�e���𐶐�����
	//	index : �C���f�b�N�X�ԍ�
	//	pos : �e���̎n�_
	//	beg_rad : �n�߂�p�x
	//	radiculus
	PY_BULLET_API void generateBullet(size_t _bullet, float _pos[2], float _beg_rad, float _radiculous);

	//	bullet_idx : �e���̃C���f�b�N�X�ԍ�
	//	bullet_id : �e���̖��O
	//	str : �e���̃f�[�^�X�g���N�`��
	PY_BULLET_API void getBulletParamData(size_t _bullet, data_base::BULLET_PARAM_DATA* const _str);
	//	�� ���̊֐��͌��݁A�@�\��񋟂��Ă��܂���!!
	PY_BULLET_API void getBulletParamData(const char* _bullet_id, data_base::BULLET_PARAM_DATA* const _str);

	//	�w�肵���e���������Ώۂ�
	//	index : �e���̔ԍ�
	PY_BULLET_API void bindBulletParamDB(size_t _bullet);

	//	�e�����X�V����
	PY_BULLET_API void runningBulletObject(float _pos[2], bool _status);

	//	�o�C���h����Ă���e���̊e�e���I�u�W�F�N�g���擾����
	PY_BULLET_API std::vector<data_base::BULLET_OBJECT_MANAGER>& getBulletObjects();

	//	���C�u�����̏�����
	PY_BULLET_API void BulletSystemInit();
	//	���C�u�����̏I������
	PY_BULLET_API void BulletSystemTerminate();
}
#endif // GLBS_PYTHON_DB_STATIC_LIB

#endif	//	PY_DB_HPP_