#pragma once
#include "pronet_memory.h"
#include "ObjectPoolList.h"
#include "Structure.h"
#include "Shader.h"

namespace pronet {
	/*
	* 境界をまたぐオブジェクトの処理
	* カスタムスマートポインタの使用…4方向分のリストを作成→メモリ消費量の増加、スマートポインタを実装した意味
	* 改善策
	* ・ストラクチャをスマートポインタで管理→理にかなっているがオブジェクトの扱いをどうしよう→単一のオブジェクトプールの実装
	* 
	* ストラクチャー
	* ・オブジェクト・・スマートポインタ
	* ・シェーダ・・スマートポインタ
	* ・テクスチャ・・インデックス
	* ・パラム・・個別
	* 
	* 方式
	* ・ロード時にスマートポインタを生成
	* ・参照する。元オブジェクトは生存範囲を外れるため削除だがメモリ自体は確保されたまま
	*/

	class Chanck_2D
	{
		PoolArray<Structure2v> structure;
		PoolArray<Object> buffer_pool;
		PoolArray<Shader> shader;
	
	public:
		Chanck_2D();

		~Chanck_2D();
	};
}

