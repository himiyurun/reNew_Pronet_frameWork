#pragma once
#include <iostream>

#include "bit.h"
#include "pnMath.h"
#include "colCell.h"

namespace pronet {
	template <typename _Ty>
	class Collision_4tree
	{
		float max_size[2];
		std::vector<ColCell<_Ty>, pnTlsfInsertSTLtype<_Ty>> cells;
		ColCell<_Ty>* begin;
		size_t level;

	public:
		Collision_4tree(std::size_t _lv = 0, float _xsize = 0.f, float _ysize = 0.f);
		~Collision_4tree();

		void init(std::size_t _lv, float _xsize, float _ysize);
	private:
		void assembley_tree();
	};

	template<typename _Ty>
	inline Collision_4tree<_Ty>::Collision_4tree(std::size_t _lv, float _xsize, float _ysize)
		: max_size{ 0.f, 0.f }
		, begin(nullptr)
		, level(0)
	{
		if (_lv) {
			init(_lv, _xsize, _ysize);
		}
	}
	template<typename _Ty>
	inline Collision_4tree<_Ty>::~Collision_4tree()
	{
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::init(std::size_t _lv, float _xsize, float _ysize)
	{
		if (!_lv) {
			throw std::runtime_error("level must be 0 else! : Collision_4tree.init(size_t, float, float)");
		}
		level = _lv;
		max_size[0] = _xsize;
		max_size[1] = _ysize;
		cells.reserve(gp_total(0x01, 0x04, _lv));
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::assembley_tree()
	{
		size_t level_max_size = 1;
		size_t array_pointer = 0;
		//	リンクするオブジェクトの配列の位置
		size_t link_array_pointer = 5;
		cells[0].make_cell(max_size[0], max_size[1], nullptr, cells[1], cells[2], cells[3], cells[4]);
		array_pointer++;
		level_max_size <<= 2;
		for (size_t lv = 1; lv < (level - 1); lv++) {
			for (size_t i = 0; i < level_max_size; i++) {
				cells[array_pointer].make_cell(max_size[0] / (1<<lv),max_size[1]/(1<<lv),)
			}
			level_max_size <<= 2;
		}
	}
}

