#include "pnMath.h"

size_t pronet::gp_total(size_t _first, size_t _ratio, size_t _count)
{
	size_t dmr = 1;
	size_t era = _ratio - 1;
	for (size_t i = 0; i < _count; i++) {
		dmr *= _ratio;
	}
	return (_first * dmr) / era;
}

size_t pronet::ap_total(size_t _first, size_t _ratio, size_t _count)
{
	size_t max(_first + _ratio * (_count - 1));
	return (_count * (_first + max)) / 2;
}

size_t pronet::multiplier(size_t _num, size_t _count)
{
	size_t num(1);
	for (size_t i = 0; i < _count; i++) {
		num *= _num;
	}
	return num;
}

float pronet::radiansf(float _degrees)
{
	return _degrees / 180.f * 3.14f;
}

double pronet::radiansd(double _degrees)
{
	return _degrees / 180.0 * 3.14;
}

