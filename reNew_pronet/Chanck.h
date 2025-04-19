#pragma once			
#include "pronet_memory.h"
#include "Collision_4tree.h"

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

	constexpr size_t strLv = 6;

	static size_t frame_counter = 0;

	enum CHANK_DIR {
		PRONET_CHANCK_NATIVE,
		PRONET_CHANCK_UP,
		PRONET_CHANCK_DOWN,
		PRONET_CHANCK_RIGHT,
		PRONET_CHANCK_LEFT,
		PRONET_CHANCK_UP_RIGHT,
		PRONET_CHANCK_UP_LEFT,
		PRONET_CHANCK_DOWN_RIGHT,
		PRONET_CHANCK_DOWN_LEFT,
		PRONET_CHANCK_DIRECTION_SIZE
	};

	struct ChanckObjectSizeDirectionInfo {
		size_t native;		//	チャンクだけに属するオブジェクトの数
		size_t up;			//	上の境界とかぶるオブジェクトの数
		size_t down;		//	下の境界とかぶるオブジェクトの数
		size_t right;		//	右の境界とかぶるオブジェクトの数
		size_t left;		//	左の境界とかぶるオブジェクトの数
		size_t up_right;	//	右上の境界とかぶるオブジェクトの数
		size_t up_left;		//	左上の境界とかぶるオブジェクトの数
		size_t down_right;	//	右下の境界とかぶるオブジェクトの数
		size_t down_left;	//	左下の境界とかぶるオブジェクトの数
	};

	void updateFrameCounter();

	size_t getFrameCount();

	template<std::size_t VBOLV, std::size_t SHDLV>
	class Chanck_2D
	{
		pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> structures;
		std::array<size_t, PRONET_CHANCK_DIRECTION_SIZE> obj_size;
		Collision_4tree<Structure2v<VBOLV, SHDLV>*> cells;

		using str_sp = pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>>;

	public:
		Chanck_2D();

		~Chanck_2D();

		void init(str_sp& strs, ChanckObjectSizeDirectionInfo* info);

		void reset();

		void draw() const;

		bool Intersect(const Player& _ply) const;

	private:

		void init_chanck_direction_object_size(ChanckObjectSizeDirectionInfo* info);

		void reset_chanck_direction_object_size();
	};

	template<std::size_t VBOLV, std::size_t SHDLV>
	pronet::Chanck_2D<VBOLV, SHDLV>::Chanck_2D()
		: obj_size()
	{
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	pronet::Chanck_2D<VBOLV, SHDLV>::~Chanck_2D()
	{
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::init(str_sp& strs, ChanckObjectSizeDirectionInfo* info)
	{
		structures = strs;
		init_chanck_direction_object_size(info);
		//	セルの初期化がメモリリークを一定確率で引き起こす可能性大
		cells.init(4, -5.f, 5.f, 10.f, 10.f);
		for (auto& a : structures) {
			cells.rigist(a.get());
		}
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::reset()
	{
		structures.reset();
		reset_chanck_direction_object_size();
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::draw() const
	{
		for (auto& a : structures) {
			a->use();
			updateGameObjectUniformParam(a->parameter());
			a->draw(getFrameCount());
		}
		cells.debug_draw();
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline bool Chanck_2D<VBOLV, SHDLV>::Intersect(const Player& _ply) const
	{
		return cells.intersect(_ply);
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::init_chanck_direction_object_size(ChanckObjectSizeDirectionInfo* info)
	{
		obj_size[PRONET_CHANCK_NATIVE] = info->native;
		obj_size[PRONET_CHANCK_UP] = info->up;
		obj_size[PRONET_CHANCK_DOWN] = info->down;
		obj_size[PRONET_CHANCK_RIGHT] = info->right;
		obj_size[PRONET_CHANCK_LEFT] = info->left;
		obj_size[PRONET_CHANCK_UP_RIGHT] = info->up_right;
		obj_size[PRONET_CHANCK_UP_LEFT] = info->up_left;
		obj_size[PRONET_CHANCK_DOWN_RIGHT] = info->down_right;
		obj_size[PRONET_CHANCK_DOWN_LEFT] = info->down_left;
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	void pronet::Chanck_2D<VBOLV, SHDLV>::reset_chanck_direction_object_size()
	{
		obj_size[PRONET_CHANCK_NATIVE] = 0;
		obj_size[PRONET_CHANCK_UP] = 0;
		obj_size[PRONET_CHANCK_DOWN] = 0;
		obj_size[PRONET_CHANCK_RIGHT] = 0;
		obj_size[PRONET_CHANCK_LEFT] = 0;
		obj_size[PRONET_CHANCK_UP_RIGHT] = 0;
		obj_size[PRONET_CHANCK_UP_LEFT] = 0;
		obj_size[PRONET_CHANCK_DOWN_RIGHT] = 0;
		obj_size[PRONET_CHANCK_DOWN_LEFT] = 0;
	}
}

