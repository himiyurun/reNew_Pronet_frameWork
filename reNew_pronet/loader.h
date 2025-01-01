#pragma once
#include "Chanck.h"
#include "readDocument.h"

namespace pronet {
	//	�`�����N�̃��[�h���s�������ɕK�v�ȏ���Ԃ�
	template<std::size_t VBOLV, std::size_t SHDLV, std::size_t CNKLV>
	class loader
	{
		std::array<Chanck_2D<VBOLV, SHDLV>, CNKLV> actv_chanck;
	private:

		static ObjectPool_Array<ObjectInfo2v> _oi2_pool;
		static ObjectPool_Array<ShaderMakeInfo> _smi_pool;
		
	};
}