#pragma once
#include "pronet_memory.h"
#include "readDocument.h"

namespace pronet {
	//	�`�����N�̃��[�h���s�������ɕK�v�ȏ���Ԃ�
	class loader
	{
		ObjectPool_Array<ObjectInfo2v> _oi2_pool;
		ObjectPool_Array<ShaderMakeInfo> _smi_pool;
		
	};
}