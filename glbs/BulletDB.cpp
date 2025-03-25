// pch.cpp: プリコンパイル済みヘッダーに対応するソース ファイル
#include "BulletDB.h"

// プリコンパイル済みヘッダーを使用している場合、コンパイルを成功させるにはこのソース ファイルが必要です。

PRONET_API void database::bullet::set_bullet(const BulletCreateInfo& _info, const pronet::poolObject_shared_ptr<Object, 6>& _obj, const pronet::poolObject_shared_ptr<Shader, 6>& _shd, const pronet::poolObject_shared_ptr<Texture, 6>& _tex)
{
	db_.emplace_back(_info, _obj, _shd, _tex);
}
