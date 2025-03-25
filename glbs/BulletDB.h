#pragma once
#include <boost/python.hpp>
#include "../reNew_pronet/BulletManager.h"

/*
* 敵のデータベース　⇚（参照）　弾幕データベース
* ↓
* 敵のスクリプト上で使用する
*/
#define DLL_EXPORTS
#ifdef DLL_EXPORTS
#define PRONET_API __declspec(dllexport)
#else
#define PRONET_API __declspec(dllimport)
#endif

namespace database {
	namespace bullet {
		extern std::vector<BulletManager, pnTlsfInsertSTLtype<BulletManager>> db_;

		extern PRONET_API void set_bullet(const BulletCreateInfo& _info, const pronet::poolObject_shared_ptr<Object, 6>& _obj, const pronet::poolObject_shared_ptr<Shader, 6>& _shd, const pronet::poolObject_shared_ptr<Texture, 6>& _tex);
	}
}