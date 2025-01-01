#pragma once
#include <stdexcept>

#include "pronet_memory.h"
#include "Object.h"
#include "Shader.h"
#include "Uniform.h"

//	オブジェクトをshared_ptr管理にすれば全て解決
//	境界線オブジェクト、生成するチャンクのオブジェクトファイルのインデックスによりオブジェクトを割り当てる。
//	ノーマルオブジェクト、生成するチャンクのオブジェクトファイルのインデックスにより

struct Structure2vParamCreateInfo {
	GLfloat location[2];	//	オブジェクトの位置
	GLfloat col_pos[2];		//	当たり判定をとる左上のオブジェクト座標
	GLfloat col_size[2];	//	当たり判定をとるサイズ
	GLfloat rotate;			//	オブジェクトの角度
};

struct Structure2vCreateInfo {
	size_t shader_index;
	size_t buffer_object_index;
	size_t texture_index;
	Structure2vParamCreateInfo param;
};

template<std::size_t VBOLV, std::size_t SHDLV>
class Structure2v
{
	pronet::poolObject_shared_ptr<Object, VBOLV> buffer_object;	//	バッファオブジェクト
	pronet::poolObject_shared_ptr<Shader, SHDLV> shader_object;	//	シェーダーのインデックス、チャンクごとに管理
	uint32_t texture_index;	//	テクスチャの番号
	
	//	パラメーター
	Structure2v_Param param;
	GLfloat col_pos[2];		//	当たり判定をとる左上のオブジェクト座標
	GLfloat col_size[2];	//	当たり判定をとるサイズ

public:
	//	コンストラクタ
	Structure2v();
	//	デストラクタ
	~Structure2v();

	//	初期化を行う
	//	new_object : 確保したオブジェクトのポインタ
	void init(Structure2vParamCreateInfo* strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index);

	//	このクラスを初期の状態に戻す
	void reset();
	//	内部のパラメーターをクリアする
	void clear();
	//	シェーダー使用する
	void use() const;
	//	描画する
	void draw() const;
	//	使用中かを取得する
	bool is_used() const;

	[[nodiscard]] const Structure2v_Param* parameter() const { return &param; }
	[[nodiscard]] uint32_t texture() const { return (texture_index - 1); }
};

template<std::size_t VBOLV, std::size_t SHDLV>
Structure2v<VBOLV, SHDLV>::Structure2v()
	: buffer_object(nullptr)
	, shader_object(nullptr)
	, texture_index(0)
	, col_pos{ 0.0f, 0.0f }
	, col_size{ 0.0f, 0.0f }
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
Structure2v<VBOLV, SHDLV>::~Structure2v()
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::init(Structure2vParamCreateInfo* strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index)
{
	if (object() == nullptr)
		throw std::bad_alloc();
	if (shader() == nullptr)
		throw std::bad_alloc();
	if (tex_index == 0)
		throw std::runtime_error("tex_index is 0 Structure.update");

	//	パラメーターの代入
	param.location[0] = strInfo->location[0]; param.location[1] = strInfo->location[1];
	param.rotate = strInfo->rotate;
	col_pos[0] = strInfo->col_pos[0]; col_pos[1] = strInfo->col_pos[1];
	col_size[0] = strInfo->col_size[0]; col_size[1] = strInfo->col_size[1];
	//	その他データの代入
	buffer_object = object;
	shader_object = shader;
	texture_index = tex_index;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::reset()
{
	clear();
	buffer_object.reset();
	shader_object.reset();
	texture_index = 0;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::clear()
{
	//	パラメーターのリセット
	param.location[0] = 0.0f; param.location[1] = 0.0f;
	param.rotate = 0.0f;
	col_pos[0] = 0.0f; col_pos[1] = 0.0f;
	col_size[0] = 0.0f; col_size[1] = 0.0f;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::use() const
{
	shader_object->use();
}

template<std::size_t VBOLV, std::size_t SHDLV>
void Structure2v<VBOLV, SHDLV>::draw() const
{
	buffer_object->draw();
}

template<std::size_t VBOLV, std::size_t SHDLV>
bool Structure2v<VBOLV, SHDLV>::is_used() const
{
	if (buffer_object == nullptr && shader_object == nullptr && texture_index == 0) {
		if ((param.location[0] == 0.0f && param.location[1] == 0.0f) &&
			(param.rotate == 0.0f) &&
			(col_pos[0] == 0.0f && col_pos[1] == 0.0f) &&
			(col_size[0] = 0.0f && col_size[1] = 0.0f)) {
			return false;
		}
		else {
			throw std::runtime_error("Structure reset not excute properly!!");
		}
	}
	return true;
}