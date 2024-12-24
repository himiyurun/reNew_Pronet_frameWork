#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPoolArray.h"
#include "glfw_Window.h"

#define PronetFrameWorkMain FrameWorkManager

using FrameWorkManager = class PronetManager;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト

class PronetManager : public glfw_Window, pnTlsf {
	pronet::PoolArray<Shader> shader;
	pronet::PoolArray<Object> object;

	GLint dimentionSize;

	pronet::ObjectPool_Array<Object> objPool;
	pronet::ObjectPool_Array<Shader> shdPool;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	デストラクタ
	~PronetManager();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader[0].init(vsrc, fsrc);
	}

	void InitObj(ObjectInfo2v *info,GLboolean index_used) {
		object[0].init(dimentionSize, info, index_used);
	}

	//	ループ内で実行する処理
	virtual void process();
};