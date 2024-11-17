#pragma once
#include <memory>

#include "Shader.h"
#include "Object.h"
#include "glfw_Window.h"

class PronetManager : public glfw_Window {
	Shader shader;

	Object object;

	const GLint dimentionSize;

public:

	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo windowInfo, GLint dimentionSize);

	//	デストラクタ
	~PronetManager();

	
	void InitShader(const char* vsrc, const char* fsrc) {
		shader.Init(vsrc, fsrc);
	}

	void InitObj(GLsizei vertexcount, const glm::vec2* vertex) {
		object.Init(dimentionSize, vertexcount, vertex);
	}

	//	ループ内で実行する処理
	virtual void process() const;
};