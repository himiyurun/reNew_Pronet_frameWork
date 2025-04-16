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
		float positionX;	//	弾幕のX座標
		float positionY;	//	弾幕のY座標
		float deg;			//	弾幕の角度
	}BULLET_OBJECT_PARAM;

	PY_BULLET_API typedef struct {
		float positionX;		//	弾幕の原点のX座標
		float positionY;		//	弾幕の原点のY座標
		float begin_degree;		//	弾幕の始まる角度
		float radiculus;		//	半径（距離）
	}BULLET_OBJECT_DATA;

	PY_BULLET_API typedef struct {
		BULLET_OBJECT_DATA data;					//	弾幕のデータ
		std::vector<BULLET_OBJECT_PARAM> bullet;	//	各弾幕の値
	}BULLET_OBJECT_MANAGER;

	PY_BULLET_API typedef struct {
		float coef;			//	係数（半径）
		float area;			//	弾幕の有効な範囲
		size_t count;		//	弾幕の発射する個数
		uint32_t interval;	//	弾幕の発射する間隔
		float rad;			//	発射する角度の範囲
		float speed;		//	弾幕の速度
	}BULLET_PARAM_DATA;

	PY_BULLET_API typedef struct{
		BULLET_PARAM_DATA data;						//	弾幕の基本情報
		std::vector<BULLET_OBJECT_MANAGER> bullets;	//	各弾幕のデータ
	}BULLET_PARAM_DB;

	//	弾幕の更新時に実行される関数
	//	pos : 座標、バインドされた敵の位置が渡される
	//	status : 当たり判定
	//	param_data : 弾幕のデータ
	//	index : 弾幕オブジェクトのインデックスが返される
	//	obj_param : その弾幕オブジェクトのパラメーター
	PY_BULLET_API typedef void(*BULLET_UPDATE_FUNC)(float _pos[2], bool _status, const BULLET_PARAM_DATA& _param_data, BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, BULLET_OBJECT_PARAM& _obj_param);

	//	弾幕の生成時に実行される関数
	//	pos : 座標、バインドされた敵の位置が渡される
	//	status : 当たり判定
	//	param_data : 弾幕のデータ
	//	beg_pos : 弾幕の原点（始点）
	//	beg_rad : 弾幕の始める角度
	//	radiculous : 弾幕の初期の半径
	//	obj_data : 新たに作成された弾幕オブジェクトのデータ
	PY_BULLET_API typedef void(*BULLET_GENERATE_FUNC)(float _pos[2], bool _status, const BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, BULLET_OBJECT_DATA& _obj_data);

	//	プログラムの実行時に実行される関数
	//	pos : 座標、バインドされた敵の位置が渡される
	//	status : 当たり判定
	//	data_base : 弾幕のデータ
	//	upd_func : 更新時に実行される関数
	//	gen_func : 生成時に実行される関数
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

	//	BULLET_PARAM_DB構造体を初期化する関数
	PY_BULLET_API BULLET_PARAM_DB initializeBulletParamDB(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);
	//	BULLET_FUNC_MANAGER構造体を初期化する関数
	PY_BULLET_API BULLET_FUNC_MANAGER initializeBulletSystemManager(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed, size_t _script, BULLET_EXECUTE_FUNC _exec, BULLET_UPDATE_FUNC _upd, BULLET_GENERATE_FUNC _gen);
	//	指定したインデックスのBULLET_FUNC_MANGER構造体を初期化する
	PY_BULLET_API void resetBulletFunctionManager(size_t _index);

	//	最後に実行した時間を取得
	PY_BULLET_API clock_t getLastClock();
	//	時間を現在の時刻に更新する
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

	//	弾幕データの基本情報
	extern data_base::BULLET_PARAM_DATA* bulparam_bind_obj;
	//	1回あたりに発射された弾幕ごとの基本情報
	extern data_base::BULLET_OBJECT_DATA* bulobj_data_bind_obj;
	//	各弾幕の情報
	extern data_base::BULLET_OBJECT_PARAM* bulobj_param_bind_obj;

	//	パイソンのスクリプトオブジェクトを作成する
	//	file_name : ファイル名
	PY_BULLET_API size_t createPythonFunctionObject(const char* _file_name);

	//	弾幕オブジェクトを初期化する関数を作る
	//	exec : 実行時に実行する関数（現在はテンプレートのあらかじめ設定された関数が実行される）
	//	gen : 弾幕生成時に実行される関数
	//	upd : 弾幕更新時に実行される関数
	//	coef : 係数
	//	count : 弾幕の生成数
	//	interval : 弾幕の発射する間隔（時間）
	//	rad : 弾幕の発射する角度の大きさ（度数法）
	//	speed : 弾幕の速度
	PY_BULLET_API size_t createPythonSetupInfo(py::object _exec, py::object _gen, py::object _upd, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);

	//	初期化情報をもとに弾幕オブジェクトを作る（番号は初期化情報の番号を引き継ぐ）
	PY_BULLET_API void createBulletObjectByPython();

	//	パイソンのスクリプトのデータベースを初期化する
	//	file_name : ファイル名
	//	str : データを書き込む構造体
	PY_BULLET_API void initializePythonFunctionObject(const char* _file_name, PYTHON_FUNCTION_OBJECT& _str);

	//	指定した関数名の関数オブジェクトを取得する
	//	index : パイソンオブジェクトの番号
	//	str : 情報を取得する構造体（参照）	
	PY_BULLET_API py::object getFunctionObject(size_t _script, const char* _func_name);

	//	指定したパイソンの関数オブジェクトをバインドする
	//	index : パイソンの関数オブジェクトのインデックス
	PY_BULLET_API void bindPythonBulletFuncObject(size_t _bulfunc);
	PY_BULLET_API void executeBindPythonBulletFuncExecute(const data_base::BULLET_PARAM_DATA& _data, data_base::BULLET_OBJECT_DATA& _obj);
	PY_BULLET_API void executeBindPythonBulletFuncGenerate(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
	PY_BULLET_API void executeBindPythonBulletFuncUpdate(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);

	//	指定したオブジェクトをバインドしてパイソンでの処理対象にする
	PY_BULLET_API void setBulletParamDataBinding(data_base::BULLET_PARAM_DATA* const _ptr);
	PY_BULLET_API void setBulletObjectDataBinding(data_base::BULLET_OBJECT_DATA* const _ptr);
	PY_BULLET_API void setBulletObjectParamBinding(data_base::BULLET_OBJECT_PARAM* const _ptr);

	//	バインドしたオブジェクトを取得する
	PY_BULLET_API data_base::BULLET_PARAM_DATA& getBulletParamDataBinding();
	PY_BULLET_API data_base::BULLET_OBJECT_DATA& getBulletObjectDataBinding();
	PY_BULLET_API data_base::BULLET_OBJECT_PARAM& getBulletObjectParamBinding();

	//	バインドしたオブジェクトに代入する
	PY_BULLET_API void assginBulletParamDataBinding(float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed);
	PY_BULLET_API void assginBulletObjectDataBinding(float _positionX, float _positionY, float _begin_degree, float _radiculus);
	PY_BULLET_API void assginBulletObjectParamBinding(float _positionX, float _positionY, float _deg);
}

namespace bullet_system {
	namespace _template {
		//	弾幕の実行時に実行する関数のテンプレート
		PY_BULLET_API void bullet_execute_main(float _pos[2], bool _status, size_t _script, data_base::BULLET_PARAM_DB& _data_base, data_base::BULLET_UPDATE_FUNC _upd_func, data_base::BULLET_GENERATE_FUNC _gen_func);
		PY_BULLET_API void bullet_execute_python(float _pos[2], bool _status, size_t _script, data_base::BULLET_PARAM_DB& _data_base, data_base::BULLET_UPDATE_FUNC _upd_func, data_base::BULLET_GENERATE_FUNC _gen_func);

		//	弾幕の生成時に実行する関数のテンプレート
		PY_BULLET_API void bullet_generate_main(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
		PY_BULLET_API void bullet_generate_python(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, float _beg_pos[2], float _beg_rad, float _radiculous, size_t _script, data_base::BULLET_OBJECT_DATA& _obj_data);
		
		//	弾幕の更新時に実行する関数のテンプレート
		PY_BULLET_API void bullet_update_main(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);
		PY_BULLET_API void bullet_update_python(float _pos[2], bool _status, const data_base::BULLET_PARAM_DATA& _param_data, data_base::BULLET_OBJECT_DATA& _obj_data, size_t _script, size_t _index, data_base::BULLET_OBJECT_PARAM& _obj_param);
		
#define EXECUTE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_execute_main
#define GENERATE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_generate_main
#define UPDATE_FUNCTION_TEMPLATE			bullet_system::_template::bullet_update_main

#define EXECUTE_FUNCTION_TEMPLATE_PYTHON	bullet_system::_template::bullet_execute_python
#define GENERATE_FUNCTION_TEMPLATE_PYTHON	bullet_system::_template::bullet_generate_python
#define UPDATE_FUNCTION_TEMPLATE_PYTHON		bullet_system::_template::bullet_update_python
	}

	//	弾幕のデータを作る
	//	bullet_id : 弾幕の名前
	//	coef : 1度に発射する弾幕の個数
	//	interval : 弾幕の発射する頻度
	//	rad : 弾幕の発射する角度
	//	speed : 弾幕の速度	
	PY_BULLET_API size_t createBulletDB(const char* _bullet_id, float _coef, float _area, size_t _count, uint32_t _interval, float _rad, float _speed, size_t _script
		, data_base::BULLET_UPDATE_FUNC _upd, data_base::BULLET_GENERATE_FUNC _gen, data_base::BULLET_EXECUTE_FUNC _exec = _template::bullet_execute_main);

	//	弾幕を削除する
	//	index : 弾幕のインデックス番号
	//	※ この関数は現在、機能を提供していません!!
	PY_BULLET_API void deleteBulletDB(size_t _bullet);

	//	弾幕を生成する
	//	index : インデックス番号
	//	pos : 弾幕の始点
	//	beg_rad : 始める角度
	//	radiculus
	PY_BULLET_API void generateBullet(size_t _bullet, float _pos[2], float _beg_rad, float _radiculous);

	//	bullet_idx : 弾幕のインデックス番号
	//	bullet_id : 弾幕の名前
	//	str : 弾幕のデータストラクチャ
	PY_BULLET_API void getBulletParamData(size_t _bullet, data_base::BULLET_PARAM_DATA* const _str);
	//	※ この関数は現在、機能を提供していません!!
	PY_BULLET_API void getBulletParamData(const char* _bullet_id, data_base::BULLET_PARAM_DATA* const _str);

	//	指定した弾幕を処理対象に
	//	index : 弾幕の番号
	PY_BULLET_API void bindBulletParamDB(size_t _bullet);

	//	弾幕を更新する
	PY_BULLET_API void runningBulletObject(float _pos[2], bool _status);

	//	バインドされている弾幕の各弾幕オブジェクトを取得する
	PY_BULLET_API std::vector<data_base::BULLET_OBJECT_MANAGER>& getBulletObjects();

	//	ライブラリの初期化
	PY_BULLET_API void BulletSystemInit();
	//	ライブラリの終了処理
	PY_BULLET_API void BulletSystemTerminate();
}
#endif // GLBS_PYTHON_DB_STATIC_LIB

#endif	//	PY_DB_HPP_