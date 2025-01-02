#pragma once
#include "readDocument.h"
#include "glfw_Window.h"
#include "Chanck.h"

static const size_t strLv = 6;

//	ウインドウのパラメーターを送信するユニフォームバッファオブジェクト
template<std::size_t VBOLV, std::size_t SHDLV>
class PronetManager : public glfw_Window, pnTlsf {
	pronet::PronetReadLoadFileList file_reader;

	GLint dimentionSize;

	pronet::ObjectPool<Object, VBOLV> objPool;
	pronet::ObjectPool<Shader, SHDLV> shdPool;
	pronet::ObjectPool<Structure2v<VBOLV, SHDLV>, strLv> strPool;
	std::array<pronet::Chanck_2D<VBOLV, SHDLV>, CHANCK_LOAD_SIZE> chanck;
	Structure2v<VBOLV, SHDLV> strna;
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

	void load();
	
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
	print_ObjectInfo2v(objInfo);
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
	chanck[0].reset();
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::load()
{
	pronet::PronetReadLoadFileList::PronetLoadChanckInfo info = file_reader.get_pnLCI(0);
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Shader, SHDLV>> so;
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Object, VBOLV>> oo;
	pronet::pnTlsf_unique_ptr<pronet::poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> str;
	so.realloc(info.shaders->size);
	oo.realloc(info.objs->size);
	for (size_t i = 0; i < so.size(); i++) {
		so[i] = InitShader(info.shaders[i].vsrc.c_str(), info.shaders[i].fsrc.c_str());
	}
	for (size_t i = 0; i < oo.size(); i++) {
		oo[i] = InitObj(&info.objs[i], GL_TRUE);
	}
	str.realloc(info.strs[pronet::CHANCK_NATIVE]->size);
	for (size_t i = 0; i < info.strs[pronet::CHANCK_NATIVE]->size; i++) {
		std::cout << "buffer_object_index : " << info.strs[pronet::CHANCK_NATIVE][i].buffer_object_index << std::endl
			<< "shader_index : " << info.strs[pronet::CHANCK_NATIVE][i].shader_index << std::endl;
		str[i] = initStr(&info.strs[pronet::CHANCK_NATIVE][i].param, 
			oo[info.strs[pronet::CHANCK_NATIVE][i].buffer_object_index], 
			so[info.strs[pronet::CHANCK_NATIVE][i].shader_index],
			info.strs[pronet::CHANCK_NATIVE][i].texture_index);
	}

	pronet::ChanckObjectSizeDirectionInfo size_dir;
	if (info.strs[pronet::CHANCK_NATIVE])
		size_dir.native = info.strs[pronet::CHANCK_NATIVE]->size;
	if (info.strs[pronet::CHANCK_BOUNDARY_UP])
		size_dir.up = info.strs[pronet::CHANCK_BOUNDARY_UP]->size;
	if (info.strs[pronet::CHANCK_BOUNDARY_DOWN])
		size_dir.down = info.strs[pronet::CHANCK_BOUNDARY_DOWN]->size;
	if (info.strs[pronet::CHANCK_BOUNDARY_RIGHT])
		size_dir.right = info.strs[pronet::CHANCK_BOUNDARY_RIGHT]->size;
	if (info.strs[pronet::CHANCK_BOUNDARY_LEFT])
		size_dir.left = info.strs[pronet::CHANCK_BOUNDARY_LEFT]->size;
	chanck[0].init(str, &size_dir);

	strna.init(&info.strs[pronet::CHANCK_NATIVE][1].param, oo[1], so[0], 1);
}

template<std::size_t VBOLV, std::size_t SHDLV>
void PronetManager<VBOLV, SHDLV>::process()
{
	pronet::updateApplicationUniformParam(&param);
	pronet::updateFrameCounter();
	chanck[0].draw();
	strna.use();
	pronet::updateGameObjectUniformParam(strna.parameter());
	strna.draw(pronet::getFrameCount());
}