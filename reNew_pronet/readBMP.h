#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <glad/glad.h>
#include "pnTlsf.h"

namespace pronet {
	class readBMP
	{
		const char* imgname_;
		BITMAPFILEHEADER fileh_;
		BITMAPINFOHEADER infoh_;
		std::vector<GLubyte, pnTlsfInsertSTLtype<GLubyte>> img_;
	public:
		//	コンストラクタ
		//	imgname : 読み込むビットマップ画像の名前
		readBMP(const char* _imgname);
		//	デストラクタ
		~readBMP();
		//	ビットマップファイルヘッダーを取得
		const BITMAPFILEHEADER& fileHead() const { return fileh_; }
		//	ビットマップインフォヘッダーを取得
		const BITMAPINFOHEADER* operator->() const { return &infoh_; }
		//	画像のピクセルデータを取得
		[[nodiscard]] const GLubyte* pixcel() const { return img_.data(); }
	};
}