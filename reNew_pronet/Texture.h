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
	//	テクスチャオブジェクトの番号を取得
	[[nodiscard]] GLuint obj() const { return tex; }
};

