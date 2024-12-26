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

	//	変数のビットの最小のビットの0を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_zero_from(uint64_t n, size_t size, size_t start, size_t* idx);

	//	変数のビットの最小のビットの1を検索する関数、見つからなかったら false を返す
	//	n : 対象の変数
	//	size : 変数のビット数
	//	start : 検索を始める位置
	//	idx : インデックスを格納する変数、見つからなかったら操作が行われない
	bool _bit_find_one_from(uint64_t n, size_t size, size_t start, size_t* idx);

	//	指定した領域のビットを抽出する関数
	//	n : 抽出したい変数
	//	size : 変数のビット数
	//	start : 抽出したい領域の位置
	//	area : 抽出したい領域の大きさ
	uint64_t _bit_extract_area(uint64_t n, size_t buf_size, size_t start, size_t area);

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
}