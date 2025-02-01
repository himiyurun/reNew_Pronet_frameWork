#include "bit.h"

void pronet::print_bit(uint64_t n, uint8_t size)
{
	for (int i = 0; i < size; i++) {
		if (n & (static_cast<uint64_t>(1) << i))
			std::cout << "1";
		else
			std::cout << "0";
	}
	std::cout << std::endl;
}

bool pronet::_bit_find_zero_from(uint64_t n, size_t size, size_t start, size_t* idx)
{
	unsigned long index(0);
	assert(start < size && "Error : _bit_find_zero_from : out of range");
	if (_BitScanForward64(&index, ~(n >> start))) {
		*idx = index + start;
		return true;
	}
	return false;
}

bool pronet::_bit_find_one_from(uint64_t n, size_t size, size_t start, size_t* idx)
{
	unsigned long index(0);
	assert(start < size && "Error : _bit_find_one_from : out of range");
	if (_BitScanForward64(&index, n >> start)) {
		*idx = index + start;
		return true;
	}
	return false;
}

uint64_t pronet::_bit_extract_area(uint64_t n, size_t buf_size, size_t start, size_t area)
{
	assert(start + area <= buf_size && "Error : _bit_extract_area : out of range");
	//	最大値にする
	//	変数のサイズからエリアを引いた分、左に動かし1を引くことでマスクを作る
	//	area 分ずらすことで右端が area 分のビットを0にする
	//	ビット反転させてマスクを作る
	uint64_t buf(~(~(uint64_t)(0) & (((1ULL << (UNSIGNED_INT_64 - area)) - 1) << area)));
	return (n >> start) & buf;
}

bool pronet::_bit_get_status(uint64_t n, size_t size, size_t point)
{
	assert(point < size && "Error : _bit_get_status : out of range");
	if (1 & (n >> point))
		return true;
	else
		return false;
}

void pronet::_bit_write_zero_area(uint64_t* n, size_t buf_size, size_t start, size_t area)
{
	assert(start + area < buf_size * 64 && "Error : BitMap64.write_Bit : out of range");
	for (size_t i = 0; i < area; i++) {
		*n &= ~(1ULL << (start + i));
	}
}

void pronet::_bit_write_one_area(uint64_t* n, size_t buf_size, size_t start, size_t area)
{
	assert(start + area < buf_size * 64 && "Error : BitMap64.write_Bit : out of range");
	for (size_t i = 0; i < area; i++) {
		*n |= 1ULL << (start + i);
	}
}

uint32_t pronet::_bit_separate_16(uint16_t _num)
{
	uint32_t out(_num);
	out = (out | (out << 8)) & 0x00ff00ff;
	out = (out | (out << 4)) & 0x0f0f0f0f;
	out = (out | (out << 2)) & 0x33333333;
	out = (out | (out << 1)) & 0x55555555;
	return out;
}

uint32_t pronet::_bit_mix_32(uint32_t _n1, uint32_t _n2)
{
	return (_n1 | (_n2 << 1));
}
