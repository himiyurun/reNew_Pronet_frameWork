#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <functional>

#include "Structure.h"

namespace pronet{
	enum loadChankDirection {
		CHANCK_NATIVE,
		CHANCK_BOUNDARY_UP,
		CHANCK_BOUNDARY_DOWN,
		CHANCK_BOUNDARY_RIGHT,
		CHANCK_BOUNDARY_LEFT,
		CHANCK_DIRECTION_SIZE
	};

	class loadPronetMap2
	{
		std::ifstream ifs;
		std::string line;
		std::string script;
		std::istringstream iss;

		const char* file_name;

		static pronet::ObjectPool_Array<Structure2vCreateInfo> pool;

	public:
		loadPronetMap2();

		//	マップファイルを読み込みストラクチャの配置を取得する
		bool get_mapInfo(const char* name, poolArray_unique_ptr<Structure2vCreateInfo> info[5]);

		//	ストラクチャの配置の情報をプールに返却する
		void return_create_info(PoolArray<Structure2vCreateInfo> info[5]);

	private:
		//	ファイルを開く
		bool read_file_init(const char* name);
		//	ファイルのタイプが合っているか
		bool type_is();
		//	ストラクチャ関数内の調査を行う
		void get_struct(poolArray_unique_ptr<Structure2vCreateInfo>& info);
		//	ストラクチャの割り当てを行う
		void structure_by_script(const char* text, poolArray_unique_ptr<Structure2vCreateInfo>& info);
		//	行ごとに文字を読み込む
		void line_getting_by_text();
		//	ファイルを読み込んでそれに応じた処理を行う
		void get_script(poolArray_unique_ptr<Structure2vCreateInfo> info[5]);
		
		//	ファイルを閉じる
		bool file_close();
		
		//	指定したスクリプトの場合実行する処理を記述
		void script_func(const char* text, std::function<void()> func);
		//	string関係のメンバ変数をクリアする
		void clear_string();
	};
}

