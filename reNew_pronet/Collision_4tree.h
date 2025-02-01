#pragma once
#include <iostream>
#include <queue>

#include "bit.h"
#include "pnMath.h"
#include "colCell.h"
#include "Structure.h"
#include "Player.h"
#include "collib.h"

namespace pronet {
	template <typename _Ty>
	class Collision_4tree
	{
		float position[2];
		float max_size[2];
		float min_w[2];
		size_t max_cell;
		std::vector<ColCell<_Ty>, pnTlsfInsertSTLtype<_Ty>> cells;
		ColCell<_Ty>* begin;
		size_t level;

		Object shape;
		Shader shader;

	public:
		Collision_4tree(std::size_t _lv = 0, float _xpos = 0.f, float _ypos = 0.f, float _xsize = 0.f, float _ysize = 0.f);
		~Collision_4tree();
		
		//	空間の初期化を行う
		//	_lv : レベル数
		//	_xsize : 横方向のサイズ
		//	_ysize : 縦方向のサイズ
		void init(std::size_t _lv, float _xpos, float _ypos, float _xsize, float _ysize);
		
		void all() {
			search_all(0, begin, Player());
		}
		
		//	すべてのツリーの要素をクリアする
		void clear_all();
		
		//	オブジェクトを特定のレベルに割り当てる
		void rigist(const _Ty* const _obj);

		void rs(const Player& _ply) const {
			const Collusion_Quad col(_ply.getColInfoQuad());
			const float* const position(_ply.position());
			uint32_t morton_num_tl(get_morton_2D(col.pos[0] + position[0], col.pos[1] + position[1]));
			uint32_t morton_num_rd(get_morton_2D(col.pos[0] + col.size[0] + position[0], col.pos[1] - col.size[1] + position[1]));
			uint8_t lv(get_rigist_level(morton_num_tl, morton_num_rd));
		}

		//	デバッグ用に分割しているところを描画する
		void debug_draw() const;
		
		//	四分木の一番頭の要素を返す
		[[nodiscard]] const ColCell<_Ty>* beg() const { return begin; }
	private:
		//	オブジェクトの持つ空間すべてで処理を行う
		//	_beg : 対象のオブジェクト
		void search_all(size_t _lv, ColCell<_Ty>* const _beg, const Player& _ply);
		
		//	空間の配列を木構造にマップする
		void assembley_tree();
		
		//	総当たりの場合に実行する処理
		void search_all_process(const Player& _ply, ColCell<_Ty>* const _ptr);
		
		//	所属するレベルを求める
		[[nodiscard]] uint8_t get_rigist_level(uint32_t _sep_tl, uint32_t _sep_rd) const;
		
		//	モートン番号を求める
		//	_x : X座標
		//	_y : Y座標
		[[nodiscard]] uint32_t get_morton_2D(float _x, float _y) const;
		
		//	空間の持つ最高レベルの子空間を取得
		[[nodiscard]] ColCell<_Ty>* bottom(ColCell<_Ty>* _obj) const;
		
		//	空間の最高レベルの空間を子空間に持つ空間を取得
		[[nodiscard]] ColCell<_Ty>* have_bottom(ColCell<_Ty>* _obj) const;
		
		//	再帰関数を使い、ポインタを割り出す
		[[nodiscard]] ColCell<_Ty>* setup_tree_stage(size_t _lv, size_t _lv_max, ColCell<_Ty>* _this, size_t& _mem_pos);
	};

	template<typename _Ty>
	inline Collision_4tree<_Ty>::Collision_4tree(std::size_t _lv, float _xpos, float _ypos, float _xsize, float _ysize)
		: position{ 0.f, 0.f }
		, max_size{ 0.f, 0.f }, min_w{ 0.f, 0.f }
		, begin(nullptr), level(0)
		, max_cell(0)
	{
		if (_lv) {
			init(_lv, _xpos, _ypos, _xsize, _ysize);
		}
	}
	template<typename _Ty>
	inline Collision_4tree<_Ty>::~Collision_4tree()
	{
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::init(std::size_t _lv, float _xpos, float _ypos, float _xsize, float _ysize)
	{
		if (!_lv) {
			throw std::runtime_error("level must be 0 else! : Collision_4tree.init(size_t, float, float)");
		}
		level = _lv;
		position[0] = _xpos;
		position[1] = _ypos;
		max_size[0] = _xsize;
		max_size[1] = _ysize;
		min_w[0] = _xsize / (1 << (level - 1));
		min_w[1] = _ysize / (1 << (level - 1));
		max_cell = 1 << (level - 1);
		cells.resize(gp_total(0x01, 0x04, _lv));
		assembley_tree();

		size_t mw((1 << (level - 1)) + 1);
		std::vector<glm::vec2> coords(multiplier(mw, 2));
		size_t counter(0);
		for (auto& a : coords) {
			a = glm::vec2(min_w[0] * (counter % mw), min_w[1] * (counter / mw));
			counter++;
		}

		std::vector<GLuint> indices(gp_total(0x01, 0x04, _lv) * 4 * 2);
		size_t width(0);
		size_t cellw;
		size_t cc(0);
		for (size_t i = 0; i < level; i++) {
			width = 1 << (level - i - 1);
			cellw = 1 << (i);
			for (size_t j = 0; j < (1 << (i * 2)); j++) {
				indices[cc++] = ((j% cellw) + (j / cellw) * mw) * width;
				indices[cc++] = ((j % cellw + 1) + (j / cellw) * mw) * width;

				indices[cc++] = ((j % cellw + 1) + (j / cellw) * mw) * width;
				indices[cc++] = ((j % cellw + 1) + (j / cellw + 1) * mw) * width;

				indices[cc++] = ((j % cellw + 1) + (j / cellw + 1) * mw) * width;
				indices[cc++] = ((j % cellw) + (j / cellw + 1) * mw) * width;

				indices[cc++] = ((j % cellw) + (j / cellw + 1) * mw) * width;
				indices[cc++] = ((j % cellw) + (j / cellw) * mw) * width;
			}
		}
		shape.init(2, coords.size(), coords.data(), indices.size(), indices.data());
		shader.init("debug_c4t.vert", "debug_c4t.frag");
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::clear_all()
	{
		std::deque<ColCell<_Ty>*, pnTlsfInsertSTLpointer<ColCell<_Ty>*>> custom_deque;
		std::queue<ColCell<_Ty>*, std::deque<ColCell<_Ty>*, pnTlsfInsertSTLpointer<ColCell<_Ty>*>>> stack(custom_deque);
		stack.push(begin);

		ColCell<_Ty>* ptr(nullptr);
		while (!stack.empty()) {
			ptr = stack.front();
			stack.pop();
			
			const auto& data = ptr->get();
			data.clear();

			if (ptr->have_kids()) {
				for (uint8_t i = 0; i < 4; i++) {
					stack.push(ptr->select(i));
				}
			}
		}
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::rigist(const _Ty* const _obj)
	{
	}
	inline void Collision_4tree<Structure2v<6, 6>>::rigist(const Structure2v<6, 6>* const _obj)
	{
		const Collusion_Quad col(_obj->getColInfoQuad());
		uint32_t morton_num_tl(get_morton_2D(col.pos[0], col.pos[1]));
		uint32_t morton_num_rd(get_morton_2D(col.pos[0] + col.size[0], col.pos[1] + col.size[1]));
		uint8_t lv(get_rigist_level(morton_num_tl, morton_num_rd));

		ColCell<Structure2v<6, 6>>* ptr(begin);
		for (uint8_t i = 0; i < lv; i++) {
			ptr = ptr->select(_bit_extract_area(morton_num_tl, UNSIGNED_INT_32, i * 2, 2));
		}
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::debug_draw() const
	{
		shader.use();
		glUniform2fv(glGetUniformLocation(shader.getProgram(), "location"), 1, position);
		shape.draw(GL_LINES);
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::search_all(size_t _lv, ColCell<_Ty>* const _beg, const Player& _ply)
	{
		std::deque<ColCell<_Ty>*, pnTlsfInsertSTLpointer<ColCell<_Ty>*>> custom_deque;
		std::queue<ColCell<_Ty>*, std::deque<ColCell<_Ty>*, pnTlsfInsertSTLpointer<ColCell<_Ty>*>>> stack(custom_deque);
		stack.push(_beg);
		
		ColCell<_Ty>* ptr(nullptr);
		while (!stack.empty()) {
			ptr = stack.front();
			stack.pop();
			search_all_process(_ply, ptr);
			if (ptr->have_kids()) {
				for (uint8_t i = 0; i < 4; i++) {
					stack.push(ptr->select(i));
				}
			}
		}
	}

	template<typename _Ty>
	inline void Collision_4tree<_Ty>::assembley_tree()
	{
		size_t level_max_size = 1;
		size_t array_pointer = 1;
		//	リンクするオブジェクトの配列の位置
		size_t link_array_pointer = 5;
		//	再帰関数を使って、木を組み立てる
		cells[0].make_cell(max_size[0], max_size[1], nullptr, 
			setup_tree_stage(1, level, &cells[0], array_pointer), setup_tree_stage(1, level, &cells[0], array_pointer), 
			setup_tree_stage(1, level, &cells[0], array_pointer), setup_tree_stage(1, level, &cells[0], array_pointer));
		begin = &cells[0];
	}

	inline void Collision_4tree<Structure2v<6, 6>>::search_all_process(const Player& _ply, ColCell<Structure2v<6, 6>>* const _ptr)
	{
		for (const auto& a : _ptr->get()) {
			pnObjectCollision(_ply.getColInfoQuad(), _ply.position(), a.getColInfoQuad(), a.location());
		}
	}

	template<typename _Ty>
	inline uint8_t Collision_4tree<_Ty>::get_rigist_level(uint32_t _sep_tl, uint32_t _sep_rd) const
	{
		uint64_t tl(0), rd(0);
		uint8_t nlv = 0;
		do {
			tl = _bit_extract_area(_sep_tl, UNSIGNED_INT_32, nlv * 2, 2);
			rd = _bit_extract_area(_sep_rd, UNSIGNED_INT_32, nlv * 2, 2);
			nlv++;
			/*
			if (nlv >= level) {
				throw std::out_of_range("this object cannot belong to this Cell : Collision_4tree.get_rigist_level(uint32_t, uint32_t)");
			}
			*/
		} while (tl != rd);
		return nlv;
	}

	inline void Collision_4tree<int>::search_all_process(const Player& _ply, ColCell<int>* const _ptr)
	{
		/*
		for (size_t i = 0; i < _ptr->get().size() + 1; i++) {
			_ptr->push_object(i);
		}

		for (const auto& a : _ptr->get()) {
			std::cout << "num " << a << std::endl;
		}
		*/
		std::cout << "hello" << std::endl;
	}

	template<typename _Ty>
	inline uint32_t Collision_4tree<_Ty>::get_morton_2D(float _x, float _y) const
	{
		uint16_t x((uint16_t)((_x - position[0]) / min_w[0]));
		uint16_t y(max_cell - (uint16_t)((_y - position[1] + max_size[1]) / min_w[1]) - 1);
		return _bit_mix_32(_bit_separate_16(x), _bit_separate_16(y));
	}

	template<typename _Ty>
	inline ColCell<_Ty>* Collision_4tree<_Ty>::bottom(ColCell<_Ty>* _obj) const
	{
		if (!_obj)
			return nullptr;

		ColCell<_Ty>* obj = _obj;
		while (obj->have_kids()) {
			obj = obj->select(0);
		}
		return obj;
	}

	template<typename _Ty>
	inline ColCell<_Ty>* Collision_4tree<_Ty>::have_bottom(ColCell<_Ty>* _obj) const
	{
		if (!_obj)
			return nullptr;

		ColCell<_Ty>* obj = _obj;
		while (obj->select(0)->have_kids) {
			obj = obj->select(0);
		}
		return obj;
	}

	template<typename _Ty>
	inline ColCell<_Ty>* Collision_4tree<_Ty>::setup_tree_stage(size_t _lv, size_t _lv_max, ColCell<_Ty>* _this, size_t& _mem_pos)
	{
		size_t now(_mem_pos);
		//	レベルに1を足した数が最高レベルよりも小さい場合は子オブジェクトを持たせる
		if (++_lv < _lv_max) {
			_mem_pos++;
			//	子オブジェクトを持つ場合は、先に子オブジェクトを再帰的に作る
			cells[now].make_cell(max_size[0] / (1 << _lv), max_size[1] / (1 << _lv), _this, 
				setup_tree_stage(_lv, _lv_max, &cells[now], _mem_pos), setup_tree_stage(_lv, _lv_max, &cells[now], _mem_pos),
				setup_tree_stage(_lv, _lv_max, &cells[now], _mem_pos), setup_tree_stage(_lv, _lv_max, &cells[now], _mem_pos));
		}
		else {
			//	最高レベルの場合は子オブジェクトを作らず関数を1つ抜ける
			cells[now].make_cell(max_size[0] / (1 << _lv), max_size[1] / (1 << _lv), _this,
				nullptr, nullptr, nullptr, nullptr);
			_mem_pos++;
		}
		return &cells[now];
	}
}

