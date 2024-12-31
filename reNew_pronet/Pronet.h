#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "Object.h"
#include "ObjectPoolArray.h"
#include "glfw_Window.h"
#include "readDocument.h"
#include "Structure.h"

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト
template<std::size_t VBOLV, std::size_t SHDLV>
class PronetManager : public glfw_Window, pnTlsf {
	pronet::PronetReadLoadFileList file_reader;

	GLint dimentionSize;

	pronet::ObjectPool<Object, VBOLV> objPool;
	pronet::ObjectPool<Shader, SHDLV> shdPool;
	Structure2v<VBOLV, SHDLV> str;

	pronet::Uniform<WindowParam> winParamUbo;

public:

	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, const char* loadfilelist_name);

	//	デストラクタ
	~PronetManager();

	pronet::poolObject_shared_ptr<Shader, SHDLV> InitShader(const char* vsrc, const char* fsrc);

	pronet::poolObject_shared_ptr<Object, VBOLV> InitObj(ObjectInfo2v* objInfo, GLboolean index_used);

	void initStr(Structure2vParamCreateInfo* strInfo, const pronet::poolObject_shared_ptr<Object, VBOLV>& object, const pronet::poolObject_shared_ptr<Shader, SHDLV>& shader, uint32_t tex_index);

	void load(Structure2vParamCreateInfo* strInfo);
	
	//	ループ内で実行する処理
	virtual void process();
};

template<std::size_t VBOLV, std::size_t SHDLV>
pronet::poolObject_shared_ptr<Shader, SHDLV> PronetManager<VBOLV, SHDLV>::InitShader(const char* vsrc, const char* fsrc)
{
	pronet::poolObject_shared_ptr<Shader, SHDLV> shader(&shdPool);
	shader->init(vsrc, fsrc);
	return shader;
}

template<std::size_t VBOLV, std::size_t SHDLV>
pronet::poolObject_shared_ptr<Object, VBOLV> PronetManager<VBOLV, SHDLV>::InitObj(ObjectInfo2v* objInfo, GLboolean index_used)
{
	pronet::poolObject_shared_ptr<Object, VBOLV> object(&objPool);
	object->init(dimentionSize, objInfo, index_used);
	return object;
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::initStr(Structure2vParamCreateInfo* strInfo, const pronet::poolObject_shared_ptr<Object, VBOLV>& object, const pronet::poolObject_shared_ptr<Shader, SHDLV>& shader, uint32_t tex_index)
{
	str.init(strInfo, object, shader, tex_index);
}

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::PronetManager(glfw_windowCreateInfo* windowInfo, const char* loadfilelist_name)
	: glfw_Window(windowInfo)
	, file_reader(loadfilelist_name, &dimentionSize)
	, winParamUbo("window")
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::~PronetManager()
{
	str.reset();
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::load(Structure2vParamCreateInfo* strInfo)
{
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo info = file_reader.get_pnLCI(0);
	std::cout << info.objs
	pronet::poolObject_shared_ptr<Shader, SHDLV> so = InitShader(info.shaders[0].vsrc.c_str(), info.shaders[0].fsrc.c_str());
	pronet::poolObject_shared_ptr<Object, VBOLV> oo = InitObj(&info.objs[0], GL_TRUE);
	initStr(strInfo, oo, so, 1);
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::process()
{
	str.use();
	winParamUbo.bind();

	winParamUbo.Update(&param, 1);

	str.draw();
}