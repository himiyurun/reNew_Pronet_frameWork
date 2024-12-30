#pragma once
#include "pronet_memory.h"
#include "readDocument.h"

namespace pronet {
	//	チャンクのロードを行い生成に必要な情報を返す
	class loader
	{
		ObjectPool_Array<ObjectInfo2v> _oi2_pool;
		ObjectPool_Array<ShaderMakeInfo> _smi_pool;
		
	};
}