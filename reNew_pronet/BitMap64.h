#pragma once
#include <vector>

#include "bit.h"

#define BITCOUNT_OF_64	(6)
#define BITMASK_OF_64	(0x3f)

namespace pronet {
	class BitMap64
	{
		std::vector<uint64_t> bit;

	public:
		//	�R���X�g���N�^
		BitMap64(size_t size)
			: bit(size, 0)
		{
		}
		//	�f�X�g���N�^
		~BitMap64() {}

		//	�ʒu���w�肵�Ă����̃r�b�g���擾����
		bool operator[](size_t _n) const {
			size_t current(getCurrent(_n));
			size_t index(getIndex(_n));

			if (bit[current] & (1ULL << index))
				return true;
			else
				return false;
		}

		//	�ʒu���w�肵�Ă����̃r�b�g���擾����B�͈͊O�A�N�Z�X���̗�O�������s��
		bool get_bit_map(size_t _n) const {
			size_t current(getCurrent(_n));
			size_t index(getIndex(_n));

			if(current >= bit.size())
				throw std::runtime_error("Error : BitMap64.find_zero_from : out of range");
			if (bit[current] & (1ULL << index))
				return true;
			else
				return false;
		}

		uint64_t extract_area(size_t _start, size_t _area) const {
			size_t current(getCurrent(_start));
			size_t offset(getIndex(_start));
			size_t maxonce(64 - offset);
			if (offset + _area < maxonce) {
				return _bit_extract_area(bit[current], UNSIGNED_INT_64, offset, _area);
			}
			uint64_t extractedl(_bit_extract_area(bit[current], UNSIGNED_INT_64, _start, maxonce));
			uint64_t extractedr(_bit_extract_area(bit[current + 1], UNSIGNED_INT_64, 0, _area - maxonce));
			return (extractedr << maxonce) | extractedl;
		}

		//	���߂�0���擾����
		bool find_zero_first(size_t* const _i) const {
			size_t current(getCurrent(*_i));
			for (const auto& a : bit) {
				if (_bit_find_zero_from(a, UNSIGNED_INT_64, 0, _i)) {
					*_i += (current << BITCOUNT_OF_64);
					return true;
				}
				current++;
			}
			return false;
		}

		//	���߂�1���擾����
		bool find_one_first(size_t* const _i) const {
			size_t current(getCurrent(*_i));
			for (const auto& a : bit) {
				if (_bit_find_one_from(a, UNSIGNED_INT_64, 0, _i)) {
					*_i += (current << BITCOUNT_OF_64);
					return true;
				}
				current++;
			}
			return false;
		}

		//	�w��ʒu���珉�߂�0���擾����
		bool find_zero_from(size_t _start, size_t* const _idx) const {
			size_t current(getCurrent(_start));
			size_t offset(getIndex(_start));
			size_t index(0);

			assert(start < bit.size() * 64 && "Error : BitMap64.find_zero_from : out of range");
			
			if (_bit_find_zero_from(bit[current], UNSIGNED_INT_64, offset, &index)) {
				*_idx = index + ((uint64_t)current << BITCOUNT_OF_64);
				return true;
			}

			for (size_t i = ++current; i < bit.size(); i++) {
				if (_bit_find_zero_from(bit[i], UNSIGNED_INT_64, 0, &index)) {
					*_idx = index + ((uint64_t)i << BITCOUNT_OF_64);
					return true;
				}
			}
			return false;
		}

		/*1�����������͈̔͊O�A�N�Z�X��h�~���邽�߂Ƀr�b�g�}�b�v�̃������̊m�ۂ̕�����ύX���������A���������Ɗm���Ƀ��������[�N����������̂ň�U�ۗ�����*/
		bool find_one_from(size_t _start, size_t* const _idx) const {
			size_t current(getCurrent(_start));
			size_t offset(getIndex(_start));
			size_t index(0);

			assert(start < bit.size() * 64 && "Error : BitMap64.find_one_from : out of range");

			if (_bit_find_one_from(bit[current], UNSIGNED_INT_64, offset, &index)) {
				*_idx = index + ((uint64_t)current << BITCOUNT_OF_64);
				return true;
			}

			for (size_t i = ++current; i < bit.size(); i++) {
				if (_bit_find_one_from(bit[i], UNSIGNED_INT_64, 0, &index)) {
					*_idx = index + ((uint64_t)i << BITCOUNT_OF_64);
					return true;
				}
			}
			return false;
		}

		void write_Bit_1(size_t _start, size_t _size) {
			size_t current(getCurrent(_start));
			size_t index(getIndex(_start));
			size_t area(_size);
			size_t maxonce(64 - index);

			assert(start < bit.size() * 64 && "Error : BitMap64.write_Bit : out of range");

			if (_size < maxonce) {
				_bit_write_one_area(&bit[current], UNSIGNED_INT_64, index, _size);
				return;
			}
			_bit_write_one_area(&bit[current], UNSIGNED_INT_64, index, maxonce);
			area -= maxonce;
			current++;
			assert(current < bit.size() && "Error : BitMap64.write_Bit : out of range");

			size_t fullmaxcurrent(getCurrent(area) + current);
			assert(fullmaxcurrent < bit.size() && "Error : BitMap64.write_Bit : out of range");
			for (size_t i = current; i < fullmaxcurrent; i++) {
				_bit_write_one_area(&bit[i], UNSIGNED_INT_64, 0, UNSIGNED_INT_64);
				area -= UNSIGNED_INT_64;
			}
			_bit_write_one_area(&bit[fullmaxcurrent], UNSIGNED_INT_64, 0, area);
		}

		void write_Bit_0(size_t _start, size_t _size) {
			size_t current(getCurrent(_start));
			size_t index(getIndex(_start));
			size_t area(_size);
			size_t maxonce(64 - index);

			assert(start < bit.size() * 64 && "Error : BitMap64.write_Bit : out of range");

			if (_size < maxonce) {
				_bit_write_zero_area(&bit[current], UNSIGNED_INT_64, index, _size);
				return;
			}
			_bit_write_zero_area(&bit[current], UNSIGNED_INT_64, index, maxonce);
			area -= maxonce;
			current++;
			assert(current < bit.size() && "Error : BitMap64.write_Bit : out of range");

			size_t fullmaxcurrent(getCurrent(area) + current);
			for (size_t i = current; i < fullmaxcurrent; i++) {
				_bit_write_zero_area(&bit[i], UNSIGNED_INT_64, 0, UNSIGNED_INT_64);
				area -= UNSIGNED_INT_64;
			}
			_bit_write_zero_area(&bit[fullmaxcurrent], UNSIGNED_INT_64, 0, area);
		}

		void resize(size_t size) {
			bit.resize(size, 0);
		}

		void printBit() const {
			for (uint64_t num : bit) {
				for (int i = 0; i < 64; i++) {
					if (num & (static_cast<uint64_t>(1) << i))
						std::cout << "1";
					else
						std::cout << "0";
				}
				std::cout << std::endl;
			}
		}

		[[nodiscard]] size_t size() const { return bit.size(); }

	private:
		[[nodiscard]] size_t getCurrent(size_t _n) const {
			return _n >> BITCOUNT_OF_64;
		}
		[[nodiscard]] size_t getIndex(size_t _n) const {
			return _n & BITMASK_OF_64;
		}
	};
}