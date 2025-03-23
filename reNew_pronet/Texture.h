#pragma once
#include "readBMP.h"

class Texture
{
	GLuint tex;
	bool is_used;
public:
	//	コンストラクタ
	//	imgname : テクスチャファイル名
	Texture(const char* _imgname = nullptr);
	//	デストラクタ
	~Texture();
	//	初期化を行う
	//	imgname : テクスチャファイル名
	void init(const char* _imgname);
	//	リセットする
	void reset();
	//	テクスチャをバインドする
	void bind() const;
	//	テクスチャをアクティブにする
	virtual void active() const;
	//	テクスチャを使用する
	//	id : テクスチャユニットのユニフォーム変数の番号
	void exe(GLuint _id) const;
	//	使用中かのステータスを取得
	[[nodiscard]] bool used() const { return is_used; }
	//	テクスチャオブジェクトの番号を取得
	[[nodiscard]] GLuint obj() const { return tex; }
};

