#pragma once
#include <stdexcept>

#include "ObjectPoolList.h"
#include "Object.h"

//	オブジェクトをshared_ptr管理にすれば全て解決
//	境界線オブジェクト、生成するチャンクのオブジェクトファイルのインデックスによりオブジェクトを割り当てる。
//	ノーマルオブジェクト、生成するチャンクのオブジェクトファイルのインデックスにより

struct Structure2vCreateInfo {
	size_t buffer_object_index;
	size_t shader_index;
	Structure2vParamCreateInfo param;
};

struct Structure2vParamCreateInfo {
	GLfloat location[2];	//	オブジェクトの位置
	GLfloat col_pos[2];		//	当たり判定をとる左上のオブジェクト座標
	GLfloat col_size[2];	//	当たり判定をとるサイズ
	GLfloat rotate;			//	オブジェクトの角度
};

struct Structure2v_Param {
	GLfloat location[2];	//	オブジェクトの位置
	GLfloat rotate;			//	オブジェクトの角度

	Structure2v_Param() : location{ 0.0f, 0.0f }, rotate(0.0f) {}
};

class Structure2v
{
	Object* buffer_object;	//	バッファオブジェクト
	uint32_t shader_index;	//	シェーダーのインデックス、チャンクごとに管理
	uint32_t texture_index;	//	テクスチャの番号
	
	//	パラメーター
	Structure2v_Param param;
	GLfloat col_pos[2];		//	当たり判定をとる左上のオブジェクト座標
	GLfloat col_size[2];	//	当たり判定をとるサイズ
	pronet::PoolArray<glm::vec2> verts;
	pronet::PoolArray<glm::vec2> uv;
	pronet::PoolArray<uint32_t> indics;

public:
	//	コンストラクタ
	Structure2v();
	//	デストラクタ
	~Structure2v();

	//	初期化を行う
	//	new_object : 確保したオブジェクトのポインタ
	void init(Object* new_object);
	//	内部のパラメータを書き換える
	//	info : オブジェクトの情報
	//	shd_index : シェーダーのインデックス
	//	tex_index : テクスチャのインデックス
	void update(Structure2vParamCreateInfo *strInfo, ObjectInfo2v* objInfo, uint32_t shd_index, uint32_t tex_index);
	//	このクラスを初期の状態に戻す
	void reset();
	//	内部のパラメーターをクリアする
	void clear();
	//	描画する
	void draw() const;

	[[nodiscard]] uint32_t shader() const { return (shader_index - 1); }
	[[nodiscard]] uint32_t texture() const { return (texture_index - 1); }
};

