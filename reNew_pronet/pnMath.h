#pragma once
#include <iostream>

namespace pronet {
	//	���䐔��̘a
	size_t gp_total(size_t _first, size_t _ratio, size_t _count);

	//	��������̘a
	size_t ap_total(size_t _first, size_t _ratio, size_t _count);

	//	2�悷��
	size_t multiplier(size_t _num, size_t _count);

	//	�x���@����ʓx�@�ɕϊ�����
	//	degrees : �ϊ�����x��
	float radiansf(float _degrees);
	double radiansd(double _degrees);
}