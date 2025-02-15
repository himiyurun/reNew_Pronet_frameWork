#include "tlsf.h"

pronet::tlsf_set::tlsf_set(uint8_t _div_size, uint8_t _lsb)
	: fli_(0)
	, sli_(_div_size * (_lsb + 1U))
	, bitdiv_(_div_size), bitsiz_(1ULL << _div_size)
{
}

pronet::tlsf_set::~tlsf_set()
{
}

void pronet::tlsf_set::rigist(size_t _size)
{
}

void pronet::tlsf_set::unrigist(uint8_t fli, uint8_t sli)
{
}

void pronet::tlsf_set::calcIndex(uint8_t& _fli, uint8_t& _sli, size_t _size) const
{
	unsigned long index;
	_BitScanReverse(&index, _size);
	_fli = static_cast<uint8_t>(index);
	calcSLI(_sli, _fli, _size);
}

bool pronet::tlsf_set::getStat(uint8_t _fli, uint8_t _sli) const
{
	size_t index((_fli << bitdiv_) + _sli);
	return sli_[index];
}

void pronet::tlsf_set::calcSLI(uint8_t& _sli, uint8_t _fli, size_t _size) const
{
	uint8_t shift(0);
	shift = _fli - bitdiv_;
	_sli = (_size & ~(1 << _fli)) >> (shift);
}
