#pragma once
#include "Collision_4tree.h"

namespace pronet {
	template<typename _Ty>
	bool Collision_4tree<_Ty>::rigist(_Ty _obj) { return false; }

	template<>
	bool Collision_4tree<Structure2v<6, 6>*>::rigist(Structure2v<6, 6>* _obj)
	{
		uint32_t index;
		uint8_t lv;
		if (!get_level_and_index(_obj->getColInfoQuad(), _obj->location(), lv, index))
			return false;

		ColCell<Structure2v<6, 6>*>* ptr(begin);
		for (uint8_t i = level; i > lv; i--) {
			ptr = ptr->select(_bit_extract_area(index, UNSIGNED_INT_32, (i - 2) * 2, 2));
		}
		ptr->push_object(_obj);
		return true;
	}

	template<typename _Ty>
	bool Collision_4tree<_Ty>::search_all_process(const Player& _ply, ColCell<_Ty>* const _ptr) const { return false; }

	template<>
	bool Collision_4tree<Structure2v<6, 6>*>::search_all_process(const Player& _ply, ColCell<Structure2v<6, 6>*>* const _ptr) const {
		for (const auto& a : _ptr->get()) {
			if (pnObjectCollision(_ply.getColInfoQuad(), _ply.position(), a->getColInfoQuad(), a->location()))
				return true;
		}
		return false;
	}
}