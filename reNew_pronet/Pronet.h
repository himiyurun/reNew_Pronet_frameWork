#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPoolArray.h"
#include "glfw_Window.h"
#include "readDocument.h"

#define PronetFrameWorkMain FrameWorkManager

using FrameWorkManager = class PronetManager;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト

class PronetManager : public glfw_Window, pnTlsf {
	pronet::PronetReadLoadFileList file_reader;

	GLint dimentionSize;

	pronet::PoolArray<Shader> shader;
	pronet::PoolArray<Object> object;
	pronet::ObjectPool_Array<Object> objPool;
	pronet::ObjectPool_Array<Shader> shdPool;
	pronet::ObjectPool_Array<glm::vec2> vertsPool;
	pronet::ObjectPool_Array<uint32_t> indexPool;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, const char* loadfilelist_name);

	//	デストラクタ
	~PronetManager();

	void load();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader[0].init(vsrc, fsrc);
	}

	void InitObj(ObjectInfo2v *info,GLboolean index_used) {
		object[0].init(dimentionSize, info, index_used);
	}

	//	ループ内で実行する処理
	virtual void process();
};