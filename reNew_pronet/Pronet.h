#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPool.h"
#include "glfw_Window.h"

#define PronetFrameWorkMain FrameWorkManager

using FrameWorkManager = class PronetManager;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト

class PronetManager : public glfw_Window, pnTlsf {
	Shader *shader;

	Object *object;

	const GLint dimentionSize;

	pronet::ObjectPool<Object> test;
	pronet::ObjectPool<Shader> testshader;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, GLint dimentionSize);

	//	デストラクタ
	~PronetManager();
	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader = testshader.pop();
		if (shader) {
			shader->Init(vsrc, fsrc);
			if (!winParamUbo.Init(shader->getProgram(), &param)) {
				throw std::runtime_error("Uniform Block not found");
			}
		}
	}

	void InitObj(ObjectInfo2v *info,GLboolean index_used) {
		object = test.pop();
		if (object) {
			object->Init(dimentionSize, info, index_used);
		}
	}

	//	ループ内で実行する処理
	virtual void process();
};