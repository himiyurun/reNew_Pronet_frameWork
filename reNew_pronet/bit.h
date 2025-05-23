#pragma once
#include <iostream>
#include <cassert>

namespace pronet {

#define UNSIGNED_INT_8	(8)
#define UNSIGNED_INT_16	(16)
#define UNSIGNED_INT_32	(32)
#define UNSIGNED_INT_64	(64)

	//	ビットを描画する
	//	n : 描画したい変数
	//	size : 変数のビット数
	void print_bit(uint64_t n, uint8_t size);
	void _bit_print(uint64_t _n);
	void _bit_print(uint32_t _n);
	void _bit_print(uint16_t _n);
	void _bit_print(uint8_t _n);


	//	変数のビットの最小のビットの0を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_zero_from(uint64_t n, size_t size, size_t start, size_t* const idx);

	//	変数のビットの最小のビットの1を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_one_from(uint64_t n, size_t size, size_t start, size_t* const idx);

	//	変数のビットの末尾の0を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_zero_from_reverse(uint64_t n, size_t size, size_t start, size_t* const idx);

	//	変数のビットの末尾の0を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_one_from_reverse(uint64_t n, size_t size, size_t start, size_t* const idx);

	//	指定した領域のビットを抽出する関数
	//	n : 抽出したい変数
	//	size : 変数のビット数
	//	start : 抽出したい領域の位置
	//	area : 抽出したい領域の大きさ
	uint64_t _bit_extract_area(uint64_t n, size_t buf_size, size_t start, size_t area);

	//	指定した位置のビットを取得する関数
	//	n : 取得したい変数
	//	size : 変数のビット数
	//	start : 取得するの位置
	bool _bit_get_status(uint64_t n, size_t size, size_t point);

	//	指定した領域に0を書き込む
	//	n : 書き込みたい変数
	//	size : 変数のビット数
	//	start : 書き込みたい領域の位置
	//	area : 書き込みたい領域の大きさ
	void _bit_write_zero_area(uint64_t* n, size_t buf_size, size_t start, size_t area);

	//	指定した領域に1を書き込む
	//	n : 書き込みたい変数
	//	size : 変数のビット数
	//	start : 書き込みたい領域の位置
	//	area : 書き込みたい領域の大きさ
	void _bit_write_one_area(uint64_t* n, size_t buf_size, size_t start, size_t area);

	//	指定したビットを1つ交互にスライドさせる
	//	_num : スライドさせる数
	uint32_t _bit_separate_16(uint16_t _num);

	//	ビットを1つずらして合成する
	//	_n1 : 合成する変数
	//	_n2 : 合成する変数
	uint32_t _bit_mix_32(uint32_t _n1, uint32_t _n2);
}