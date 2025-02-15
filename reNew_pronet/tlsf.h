#pragma once
#include "BitMap64.h"

namespace pronet {
	class tlsf_set
	{
		uint64_t fli_;		//	FLI
		BitMap64 sli_;		//	SLI
		uint8_t bitdiv_;	//	SLIのインデックスのビット数
		size_t bitsiz_;		//	SLIのインデックスに分類できる最小値
	public:
		tlsf_set(uint8_t _div_size, uint8_t _lsb);
		~tlsf_set();

		void rigist(size_t _size);
		void unrigist(uint8_t fli, uint8_t sli);
		
		void calcIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const;

		[[nodiscard]] bool getStat(uint8_t _fli, uint8_t _sli) const;
	private:
		void calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const;
	};
}

