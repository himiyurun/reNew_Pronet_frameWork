#pragma once
#include <array>
#include <vector>

#include "bit.h"

namespace pronet {
	//	N : ツリーのレベル
	template<std::size_t N>
	//	ツリー状にビットマップする
	//	2のべき乗でレベルが分けられる
	//	N : ツリーのレベル
	class bit_tree
	{
		std::array<std::vector<uint64_t>, N> tree;

		size_t tree_size;

		bool val;

	public:
		//	コンストラクタ
		bit_tree(bool val, size_t size);
		//	デストラクタ
		~bit_tree();

		//	ビットマップのサイズを変更する
		//	size : 変更したいサイズ
		void resize(size_t size);

		//	指定されたインデックスにレジストする
		//	index : インデックス
		bool rigist(size_t *index);

		//	指定されたインデックスにアンレジストする
		//	index : インデックス
		bool unrigist(size_t index);

		void printAllBit() const;

	private:
		//	あいているインデックスを検索
		bool search_free_block(size_t* idx) const;

		//	指定されたインデックスのフリーブロックを検索
		//	level : インデックスのレベル
		//	index : インデックス
		bool serch_block_index(size_t level, size_t index, size_t* next_idx) const;

		//	使用する部分を開放する
		void unlock_bit_map(uint8_t level, size_t size);
	};

	template<std::size_t N>
	inline bit_tree<N>::bit_tree(bool val, size_t size)
		: val(val)
		, tree_size(0)
	{
		resize(size);
	}

	template<std::size_t N>
	inline bit_tree<N>::~bit_tree()
	{
	}

	template<std::size_t N>
	inline void bit_tree<N>::resize(size_t size)
	{
		uint8_t level(0);
		for (std::vector<uint64_t> &a : tree) {
			//	使用する部分を開放する
			unlock_bit_map(level, size);

			if ((size >> level) > UNSIGNED_INT_64 * a.size()) {
				//	ツリーのサイズを変更する
				if (size % UNSIGNED_INT_64 == 0) {
					if (val == false)
						a.resize(size / UNSIGNED_INT_64, 0);
					else
						a.resize(size / UNSIGNED_INT_64, ~(0));
				}
				else {
					if (val == false)
						a.resize(size / UNSIGNED_INT_64, 0);
					else
						a.resize(size / UNSIGNED_INT_64, ~(0));
				}

				uint32_t current(0);
				//	未使用の部分をマスクする
				for (uint64_t &b : a) {
					if (current * UNSIGNED_INT_64 > size) {
						if (val == false)
							_bit_write_one_area(&b, UNSIGNED_INT_64, UNSIGNED_INT_64 - (current * UNSIGNED_INT_64 - size), current * UNSIGNED_INT_64 - size);
						else
							_bit_write_zero_area(&b, UNSIGNED_INT_64, UNSIGNED_INT_64 - (current * UNSIGNED_INT_64 - size), current * UNSIGNED_INT_64 - size);
					}
					current++;
				}
			}
			level++;
		}
		tree_size = size;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::rigist(size_t *index)
	{
		size_t idx(0);
		if (!search_free_block(&idx))
			return false;

		uint8_t level(0);
		size_t now_index(0);
		uint64_t bit_area;
		for (std::vector<uint64_t> &a : tree) {
			now_index = idx >> level;
			if (!val)
				_bit_write_one_area(&tree[level][now_index / UNSIGNED_INT_64], UNSIGNED_INT_64, now_index, 1);
			else
				_bit_write_zero_area(&tree[level][now_index / UNSIGNED_INT_64], UNSIGNED_INT_64, now_index, 1);

			bit_area = _bit_extract_area(tree[level][now_index / UNSIGNED_INT_64], UNSIGNED_INT_64, now_index, 1 << (N - level - 1));
			if (!val) {
				if (bit_area == _bit_extract_area(~(0ULL), UNSIGNED_INT_64, 0, 1 << (N - (size_t)level - 1))) {
					_bit_write_one_area(&tree[level + 1][(now_index >> 1) / UNSIGNED_INT_64], UNSIGNED_INT_64, now_index >> 1, 1);
				}
			}
			else {
				if (bit_area == 0) {
					_bit_write_zero_area(&tree[level + 1][(now_index >> 1) / UNSIGNED_INT_64], UNSIGNED_INT_64, now_index >> 1, 1);
				}
			}

			level++;
		}
		*index = idx;
		return true;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::unrigist(size_t index)
	{
		return false;
	}

	template<std::size_t N>
	inline void bit_tree<N>::printAllBit() const
	{
		if (tree_size == 0) std::cerr << "tree_size is 0" << std::endl;
		for (std::vector<uint64_t> a : tree) {
			for (uint64_t b : a) {
				print_bit(b, UNSIGNED_INT_64);
			}
		}
	}

	template<std::size_t N>
	inline bool bit_tree<N>::search_free_block(size_t* idx) const
	{
		uint8_t level(N - 1);
		size_t index(0);
		for (uint64_t a : tree[level]) {
			if (!val) {
				if (!_bit_find_zero_from(a, UNSIGNED_INT_64, 0, &index))
					break;
			}
			else {
				if (!_bit_find_one_from(a, UNSIGNED_INT_64, 0, &index))
					break;
			}
		}
		if (!index)
			return false;

		level--;

		for (int i = level; i >= 0; i--) {
			if (!serch_block_index(i, index, &index))
				return false;
		}

		*idx = index;
		return true;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::serch_block_index(size_t level, size_t index, size_t* next_idx) const
	{
		uint32_t current(index << 1 / UNSIGNED_INT_64);
		size_t begin((index << 1) - UNSIGNED_INT_64 * current);
		if (!val) {
			if (!_bit_find_zero_from(tree[level][current], UNSIGNED_INT_64, begin, next_idx))
				return false;
		}
		else {
			if (!_bit_find_one_from(tree[level][current], UNSIGNED_INT_64, begin, next_idx))
				return false;
		}

		return true;
	}

	template<std::size_t N>
	inline void bit_tree<N>::unlock_bit_map(uint8_t level, size_t size)
	{
		if (tree_size == 0) return;
		//	ビットを解放する
		size_t end_bit = (tree[level].size() - 1) * UNSIGNED_INT_64 - (tree_size >> level);
		size_t bit_count = (size >> level) - (tree_size >> level);

		if (val == false)
			_bit_write_zero_area(&tree[level].back(), UNSIGNED_INT_64, end_bit, bit_count);
		else
			_bit_write_one_area(&tree[level].back(), UNSIGNED_INT_64, end_bit, bit_count);
	}
}