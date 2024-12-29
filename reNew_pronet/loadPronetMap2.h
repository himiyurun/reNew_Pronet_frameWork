#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <functional>

#include "Structure.h"

namespace pronet{
	class loadPronetMap2
	{
		std::ifstream ifs;
		std::string line;
		std::string script;
		std::istringstream iss;

		const char* file_name;
	public:
		//	マップファイルを読み込みストラクチャの配置を取得する
		bool get_map_info(const char* name, std::unique_ptr<Structure2vCreateInfo>& info);

	private:
		//	ファイルを開く
		bool read_file_init(const char* name);

		//	ファイルのタイプが合っているか
		bool type_is();
		//	ファイルを読み込んでそれに応じた処理を行う
		void get_script(std::unique_ptr<Structure2vCreateInfo>& info);
		
		//	ファイルを閉じる
		bool file_close();
		
		//	指定したスクリプトの場合実行する処理を記述
		void script_func(const char* text, std::function<void()> func);
		//	string関係のメンバ変数をクリアする
		void clear_string();
	};
}

