#pragma once
#include <iostream>

namespace pronet {
	//	等比数列の和
	size_t gp_total(size_t _first, size_t _ratio, size_t _count);

	//	等差数列の和
	size_t ap_total(size_t _first, size_t _ratio, size_t _count);

	//	2乗する
	size_t multiplier(size_t _num, size_t _count);

	//	度数法から弧度法に変換する
	//	degrees : 変換する度数
	float radiansf(float _degrees);
	double radiansd(double _degrees);
}