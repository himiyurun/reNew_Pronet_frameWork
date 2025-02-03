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

		//	自動でアンレジストする、インデックスが指定されていないので速度は遅い
		//	idx : 自動で算出したインデックスを格納する変数ポインタ、デフォルトだと nullptr が代入され保存されない
		bool unrigist_auto(size_t* idx = nullptr);

		//	ビットを描画
		void printAllBit() const;

	private:
		//	あいているインデックスを検索
		bool search_free_block(size_t* idx) const;

		//	指定されたインデックスのフリーブロックを検索
		//	level : インデックスのレベル
		//	index : インデックス
		bool serch_block_index(bool val, size_t level, size_t index, size_t* next_idx) const;

		//	使用する部分を開放する
		void unlock_bit_map(uint8_t level, size_t size);

		//	指定したインデックスに0を書き込む
		void write_index_zero_from(size_t idx);

		//	指定したインデックスに1を書き込む
		void write_index_one_from(size_t idx);
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
		//	使用する部分を開放する
		unlock_bit_map(0, size);

		uint8_t level(0);
		for (std::vector<uint64_t> &a : tree) {

			if (((size + UNSIGNED_INT_64 - 1) >> level) > UNSIGNED_INT_64 * a.size()) {
				//	ツリーのサイズを変更する
				if (val == false)
					a.resize(((size >> level) + UNSIGNED_INT_64 - 1) / UNSIGNED_INT_64, 0);
				else
					a.resize(((size >> level) + UNSIGNED_INT_64 - 1) / UNSIGNED_INT_64, ~(0));
			}
			level++;
		}

		//	未使用の部分をマスクする				
		size_t miss_size(tree[0].size() * UNSIGNED_INT_64 - size);
		assert(miss_size < 64 && "Error : bit_tree.resize : miss_size is lager than UNSIGNED_INT_64 bufsize");
		for (int i = size; i < (tree[0].size() * UNSIGNED_INT_64); i++) {
			if (!val)
				write_index_one_from(i);
			else
				write_index_zero_from(i);
		}
		tree_size = size;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::rigist(size_t *index)
	{
		size_t idx(0);
		if (!search_free_block(&idx))
			return false;

		assert(idx < tree_size && "Error : bit_tree.rigist(size_t) : out of range It must be bug!!");

		if (!val)
			write_index_one_from(idx);
		else
			write_index_zero_from(idx);

		*index = idx;
		return true;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::unrigist(size_t index)
	{
		assert((tree[0].size() * UNSIGNED_INT_64) > index && "Error : bit_tree.unrigist(size_t) : out of range");
		size_t now_index(0);
		size_t current(0);
		//	1レベルごとに解放の処理をおこなう、1つでも使用可能な物があるなら元のビットに書き換えれるので、無条件で書き換える
		for (int i = N - 1; i >= 0; i--) {
			current = (index >> i) / UNSIGNED_INT_64;
			now_index = (index >> i) % UNSIGNED_INT_64;
			if (!val) {
				if (_bit_get_status(tree[i][current], UNSIGNED_INT_64, now_index))
					_bit_write_zero_area(&tree[i][current], UNSIGNED_INT_64, now_index, 1);
			}
			else {
				if (!_bit_get_status(tree[i][current], UNSIGNED_INT_64, now_index))
					_bit_write_one_area(&tree[i][current], UNSIGNED_INT_64, now_index, 1);
			}
		}
		return true;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::unrigist_auto(size_t *idx)
	{
		uint8_t level(N - 1);
		size_t index(0);
		bool unrigist_status(false);
		//	最上位レベルから、使用中のブロックを求められる場合は、先頭にアンレジストする
		for (uint64_t a : tree[level]) {
			if (!val) {
				if (_bit_find_one_from(a, UNSIGNED_INT_64, 0, &index)) {
					if ((index << level) < tree_size) {
						unrigist(index << level);
						unrigist_status = true;
						return unrigist_status;
					}
				}
			}
			else {
				if (_bit_find_zero_from(a, UNSIGNED_INT_64, 0, &index)) {
					if ((index << level) < tree_size) {
						unrigist(index << level);
						unrigist_status = true;
						return unrigist_status;
					}
				}
			}
		}

		uint32_t current(0);
		//	見つからない場合は線形検索を行う
		for (uint64_t a : tree[0]) {
			if (!val) {
				if (_bit_find_one_from(a, UNSIGNED_INT_64, 0, &index)) {
					if ((index + current * UNSIGNED_INT_64) < tree_size) {
						unrigist_status = unrigist(index);
						return unrigist_status;
					}
				}
			}
			else {
				if (_bit_find_one_from(a, UNSIGNED_INT_64, 0, &index)) {
					if ((index + current * UNSIGNED_INT_64) < tree_size) {
						unrigist_status = unrigist(index);
						return unrigist_status;
					}
				}
			}
			current++;
		}
		if (idx)
			*idx = index;
		return unrigist_status;
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
		//	最上位レベルのどこに属しているのかを求める
		uint8_t level(N - 1);
		size_t index(0);
		for (uint64_t a : tree[level]) {
			if (!val) {
				if (_bit_find_zero_from(a, UNSIGNED_INT_64, 0, &index))
					break;
			}
			else {
				if (_bit_find_one_from(a, UNSIGNED_INT_64, 0, &index))
					break;
			}
		}

		level--;
		
		//	最下位レベルのインデックスを求める
		for (int i = level - 1; i >= 0; i--) {
			if (!serch_block_index(val, i, index, &index)) {
				return false;
			}
		}

		*idx = index;
		return true;
	}

	template<std::size_t N>
	inline bool bit_tree<N>::serch_block_index(bool val, size_t level, size_t index, size_t* next_idx) const
	{
		//	1段階下のレベルのインデックスを取得
		uint32_t current((index << 1) / UNSIGNED_INT_64);
		size_t begin((index << 1) % UNSIGNED_INT_64);
		//	空いている位置を突き止める
		if (!val) {
			if (!_bit_find_zero_from(tree[level][current], UNSIGNED_INT_64, begin, next_idx))
				return false;
		}
		else {
			if (!_bit_find_one_from(tree[level][current], UNSIGNED_INT_64, begin, next_idx))
				return false;
		}
		*next_idx += current * UNSIGNED_INT_64;

		return true;
	}

	template<std::size_t N>
	inline void bit_tree<N>::unlock_bit_map(uint8_t level, size_t size)
	{
		if (tree_size == 0) return;
		
		//	マスクしていた部分をアンレジストして開放する
		for (int i = tree_size; i < (tree[0].size() * UNSIGNED_INT_64); i++) {
			unrigist(i);
		}
	}

	template<std::size_t N>
	inline void bit_tree<N>::write_index_zero_from(size_t idx)
	{
		assert(_bit_get_status(tree[0][idx / UNSIGNED_INT_64], UNSIGNED_INT_64, idx % UNSIGNED_INT_64) && "Error : bit_tree.write_zero_from : It is zero");
		uint8_t level(0);
		size_t now_index(idx);
		size_t next_index(idx);
		uint64_t bit_area;

		_bit_write_one_area(&tree[0][idx / UNSIGNED_INT_64], UNSIGNED_INT_64, idx % UNSIGNED_INT_64, 1);
		for (int i = 1; i < N; i++) {
			//	インデックスを計算し、それがuint64_t型の配列のどの位置なのかを計算
			now_index = (idx >> (i - 1)) / UNSIGNED_INT_64;
			next_index = (idx >> i) / UNSIGNED_INT_64;
			//	指定された範囲のインデックスを取得して、要素がいっぱいになっていないかを確認
			//	1つ下の段階のビットマップ2つを抽出して、比較することで使用中にするか判断する
			bit_area = _bit_extract_area(tree[i - 1][now_index], UNSIGNED_INT_64, ((idx >> i) * 2) % UNSIGNED_INT_64, 2);

			if (bit_area == 0) {
				assert(_bit_get_status(tree[i][(next_index) / UNSIGNED_INT_64], UNSIGNED_INT_64, (idx >> i) % UNSIGNED_INT_64) && "Error : bit_tree.write_zero_from : It is zero");
				_bit_write_zero_area(&tree[i][(next_index) / UNSIGNED_INT_64], UNSIGNED_INT_64, (idx >> i) % UNSIGNED_INT_64, 1);
			}
			level++;
		}
	}

	template<std::size_t N>
	inline void bit_tree<N>::write_index_one_from(size_t idx)
	{
		assert(!_bit_get_status(tree[0][idx / UNSIGNED_INT_64], UNSIGNED_INT_64, idx % UNSIGNED_INT_64) && "Error : bit_tree.write_one_from : It is one");
		uint8_t level(0);
		size_t now_index(idx);
		size_t next_index(idx);
		uint64_t bit_area;
		//	マスクを作成
		uint64_t area_mask(_bit_extract_area(~(0ULL), UNSIGNED_INT_64, 0, 2));

		_bit_write_one_area(&tree[0][idx / UNSIGNED_INT_64], UNSIGNED_INT_64, idx % UNSIGNED_INT_64, 1);
		for (int i = 1; i < N; i++) {
			//	インデックスを計算し、それがuint64_t型の配列のどの位置なのかを計算
			now_index = (idx >> (i - 1)) / UNSIGNED_INT_64;
			next_index = (idx >> i) / UNSIGNED_INT_64;
			//	指定された範囲のインデックスを取得して、要素がいっぱいになっていないかを確認
			//	1つ下の段階のビットマップ2つを抽出して、比較することで使用中にするか判断する
			bit_area = _bit_extract_area(tree[i - 1][now_index], UNSIGNED_INT_64, ((idx >> i) * 2) % UNSIGNED_INT_64, 2);

			if (bit_area == area_mask) {
				assert(!_bit_get_status(tree[i][next_index / UNSIGNED_INT_64], UNSIGNED_INT_64, (idx >> i) % UNSIGNED_INT_64) && "Error : bit_tree.write_one_from : It is one");
				_bit_write_one_area(&tree[i][next_index], UNSIGNED_INT_64, (idx >> i) % UNSIGNED_INT_64, 1);
			}

			level++;
		}
	}
}