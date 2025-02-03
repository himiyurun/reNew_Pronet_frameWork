#pragma once
#include <iostream>
#include <vector>
#include <cassert>

#include "pnTlsf.h"

namespace pronet {

	template<class _Ty>
	class ColCell {
		float size[2];
		std::vector<_Ty, pnTlsfInsertSTLpointer<_Ty>> data;
		ColCell* kids[4];
		ColCell* prev;
		bool have_object;

	public:
		ColCell(float _xsize = 0.f, float _ysize = 0.f, ColCell* _prev = nullptr, ColCell* _kids1 = nullptr, ColCell* _kids2 = nullptr, ColCell* _kids3 = nullptr, ColCell* _kids4 = nullptr);
		virtual ~ColCell();

		void make_cell(float _xsize, float _ysize, ColCell* _prev, ColCell* _kids1, ColCell* _kids2, ColCell* _kids3, ColCell* _kids4);
		void delete_cell();

		void push_object(_Ty _obj);
		void clear_object();

		[[nodiscard]] ColCell<_Ty>* select(size_t n) const {
			if (n < 4)
				return kids[n];
			else
				throw std::out_of_range("out of range : ColCell.next_object(size_t)");
		}
		[[nodiscard]] ColCell<_Ty>* select_rap(size_t n) const {
			return kids[n % 4];
		}

		[[nodiscard]] bool empty() const {
			if (!kids[0] && !kids[1] && !kids[2] && !kids[3] &&
				!prev)
			{
				return true;
			}
			return false;
		}
		[[nodiscard]] bool have_prev() const {
			if (prev)
				return true;
			else
				return false;
		}
		[[nodiscard]] bool have_kids() const {
			if (kids[0] && kids[1] && kids[2] && kids[3])
				return true;
			else
				return false;
		}
		[[nodiscard]] ColCell<_Ty>* prev_object() const { return prev; }
		[[nodiscard]] const std::vector<_Ty, pnTlsfInsertSTLpointer<_Ty>>& get() const { return data; }
	};

	template<class _Ty>
	inline ColCell<_Ty>::ColCell(float _xsize, float _ysize, ColCell* _prev, ColCell* _kids1, ColCell* _kids2, ColCell* _kids3, ColCell* _kids4)
	{
		make_cell(_xsize, _ysize, _prev, _kids1, _kids2, _kids3, _kids4);
	}
	template<class _Ty>
	inline ColCell<_Ty>::~ColCell()
	{
		delete_cell();
	}

	template<class _Ty>
	inline void ColCell<_Ty>::make_cell(float _xsize, float _ysize, ColCell* _prev, ColCell* _kids1, ColCell* _kids2, ColCell* _kids3, ColCell* _kids4)
	{
		size[0] = _xsize;
		size[1] = _ysize;
		prev = _prev;
		kids[0] = _kids1;
		kids[1] = _kids2;
		kids[2] = _kids3;
		kids[3] = _kids4;
	}
	template<class _Ty>
	inline void ColCell<_Ty>::delete_cell()
	{
		size[0] = 0.f;
		size[1] = 0.f;
		prev = nullptr;
		kids[0] = nullptr;
		kids[1] = nullptr;
		kids[2] = nullptr;
		kids[3] = nullptr;
	}

	template<class _Ty>
	inline void ColCell<_Ty>::push_object(_Ty _obj)
	{
		data.emplace_back(_obj);
	}
	template<class _Ty>
	inline void ColCell<_Ty>::clear_object()
	{
		data.clear();
	}
}