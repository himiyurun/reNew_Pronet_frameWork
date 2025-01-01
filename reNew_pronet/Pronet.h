#pragma once
#include "Uniform.h"
#include "Shader.h"
#include "glfw_Window.h"
#include "readDocument.h"
#include "Structure.h"

static const size_t strLv = 6;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト
template<std::size_t VBOLV, std::size_t SHDLV>
class PronetManager : public glfw_Window, pnTlsf {
	pronet::PronetReadLoadFileList file_reader;

	GLint dimentionSize;

	pronet::ObjectPool<Object, VBOLV> objPool;
	pronet::ObjectPool<Shader, SHDLV> shdPool;
	pronet::ObjectPool<Structure2v<VBOLV, SHDLV>, strLv> strPool;
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> strs;
public:
	//	コンストラクタ
	//	windowInfo : 作成するウインドウの情報
	//	dimentionSize : 作成するゲームの次元
	PronetManager(glfw_windowCreateInfo *windowInfo, const char* loadfilelist_name);

	//	デストラクタ
	~PronetManager();

	pronet::poolObject_shared_ptr<Shader, SHDLV> InitShader(const char* vsrc, const char* fsrc);

	pronet::poolObject_shared_ptr<Object, VBOLV> InitObj(ObjectInfo2v* objInfo, GLboolean index_used);

	pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> initStr(Structure2vParamCreateInfo* strInfo, const pronet::poolObject_shared_ptr<Object, VBOLV>& object, const pronet::poolObject_shared_ptr<Shader, SHDLV>& shader, uint32_t tex_index);

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
	objInfo->reset();
	return object;
}

template<std::size_t VBOLV, std::size_t SHDLV>
pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> PronetManager<VBOLV, SHDLV>::initStr(Structure2vParamCreateInfo* strInfo, const pronet::poolObject_shared_ptr<Object, VBOLV>& object, const pronet::poolObject_shared_ptr<Shader, SHDLV>& shader, uint32_t tex_index)
{
	pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> structure(&strPool);
	structure->init(strInfo, object, shader, tex_index);
	return structure;
}

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::PronetManager(glfw_windowCreateInfo* windowInfo, const char* loadfilelist_name)
	: glfw_Window(windowInfo)
	, file_reader(loadfilelist_name, &dimentionSize)
{
	pronet::initUniformBlock();
}

template<std::size_t VBOLV, std::size_t SHDLV>
PronetManager<VBOLV, SHDLV>::~PronetManager()
{
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::load(Structure2vParamCreateInfo* strInfo)
{
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo info = file_reader.get_pnLCI(0);
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Shader, SHDLV>> so(info.shaders->size);
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Object, VBOLV>> oo(info.objs->size);
	for (size_t i = 0; i < info.shaders->size; i++) {
		so[i] = InitShader(info.shaders[i].vsrc.c_str(), info.shaders[i].fsrc.c_str());
	}
	for (size_t i = 0; i < info.objs->size; i++) {
		oo[i] = InitObj(&info.objs[i], GL_TRUE);
	}
	strs.realloc(info.strs[pronet::CHANCK_NATIVE]->size);
	for (size_t i = 0; i < info.strs[pronet::CHANCK_NATIVE]->size; i++) {
		strs[i] = initStr(&info.strs[pronet::CHANCK_NATIVE][i].param, 
			oo[info.strs[pronet::CHANCK_NATIVE][i].buffer_object_index], 
			so[info.strs[pronet::CHANCK_NATIVE][i].shader_index],
			info.strs[pronet::CHANCK_NATIVE][i].texture_index);
	}
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::process()
{
	pronet::updateApplicationUniformParam(&param);
	
	for (auto a : strs) {
		a->use();
		pronet::updateGameObjectUniformParam(a->parameter());
		a->draw();
	}
}