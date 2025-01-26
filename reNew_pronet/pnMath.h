#pragma once
#include <iostream>

namespace pronet {
	size_t gp_total(size_t _first, size_t _ratio, size_t _count) {
		size_t dmr = 1;
		size_t era = _ratio - 1;
		for (size_t i = 0; i < _count; i++) {
			dmr *= _ratio;
		}
		return (_first * dmr) / era;
	}
}