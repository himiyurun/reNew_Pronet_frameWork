#pragma once
#include <vector>
#include <new>

#include "BoundaryTag.h"

namespace pronet {

	class TLSFmemory {
		std::vector<uint8_t> pool;

		std::vector<uint16_t> parititionSLI;
		uint64_t parititionFLI;

		uint32_t bufSize;

		uint8_t minSize;

		uint8_t divBit;
		uint8_t divSize;

	public:

		TLSFmemory(uint32_t size, uint8_t divsize);

		~TLSFmemory();

		void writeParititionBit(uint32_t size);

		void clearParititionBit(uint32_t size);

		void writeBit(uint8_t fli, uint8_t sli);

		void clearBit(uint8_t fli, uint8_t sli);

		void calcTlsfIndex(uint8_t* fli, uint8_t* sli, uint32_t size) const;

		void calcSLI(uint8_t* sli, uint8_t fli, uint32_t size) const;

		void printBit(uint64_t num, uint8_t size) const;

		[[nodiscard]] bool fli_used(uint8_t fli) const;
		[[nodiscard]] bool sli_used(uint8_t fli, uint8_t sli) const;
		[[nodiscard]] uint32_t size() const { return bufSize; }
	};

}