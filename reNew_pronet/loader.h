#pragma once
#include "Chanck.h"
#include "readDocument.h"

namespace pronet {
	struct loadInfo {
		bool UP_CHANCK_LOADED;
		bool DOWN_CHANCK_LOADED;
		bool RIGHT_CHANCK_LOADED;
		bool LEFT_CHANCK_LOADED;
		bool UP_RIGHT_CHANCK_LOADED;
	};
	//	チャンクのロードを行い生成に必要な情報を返す
	template<std::size_t VBOLV, std::size_t SHDLV>
	class loader : public pnTlsf
	{
		PronetReadLoadFileList reader;
		int *const dimentionSize;
	public:

		loader(int* const _dimentionSize, const char* load_file_list_name) : dimentionSize(_dimentionSize), reader(load_file_list_name, _dimentionSize) {}
		~loader() {}

		void load_chanck(size_t chanck_index, uint8_t chanck_loaded, Chanck_2D<VBOLV, SHDLV>& chanck);
		void load_playerinfo(const char* name, Player2vCreateInfo* const info);
	private:

		poolObject_shared_ptr<Object, VBOLV> init_object
		(ObjectInfo2v* const _info, bool _index_used, bool _texture_used) const;

		poolObject_shared_ptr<Shader, SHDLV> init_shader
		(const char* const _vsrc, const char* const _fsrc) const;

		poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> init_structure
		(const Structure2vParamCreateInfo* const strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object
			, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index) const;
	private:

		//static ObjectPool_Array<poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> sp_array;

		static ObjectPool<Object, VBOLV> obj_pool;
		static ObjectPool<Shader, SHDLV> shd_pool;
		static ObjectPool<Structure2v<VBOLV, SHDLV>, strLv> str_pool;
	};

	template<std::size_t VBOLV, std::size_t SHDLV>
	ObjectPool<Shader, SHDLV> pronet::loader<VBOLV, SHDLV>::shd_pool(32);
	template<std::size_t VBOLV, std::size_t SHDLV>
	ObjectPool<Object, VBOLV> pronet::loader<VBOLV, SHDLV>::obj_pool(32);
	template<std::size_t VBOLV, std::size_t SHDLV>
	ObjectPool<Structure2v<VBOLV, SHDLV>, strLv> pronet::loader<VBOLV, SHDLV>::str_pool(32);


	template<std::size_t VBOLV, std::size_t SHDLV>
	inline void loader<VBOLV, SHDLV>::load_chanck(size_t chanck_index, uint8_t chanck_loaded, Chanck_2D<VBOLV, SHDLV>& chanck)
	{
		//	ロードするファイルを取得
		PronetReadLoadFileList::PronetLoadChanckInfo info;
		reader.get_pnLCI(chanck_index, info);
		//	シェーダーとバッファオブジェクトを作成
		pnTlsf_unique_ptr<poolObject_shared_ptr<Object, VBOLV>> object(info.objs->size);
		pnTlsf_unique_ptr<poolObject_shared_ptr<Shader, SHDLV>> shader(info.shaders->size);
		pnTlsf_unique_ptr<poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv>> structures(info.strs[pronet::CHANCK_NATIVE]->size);
		
		//	初期化を行う
		for (size_t i = 0; i < info.objs->size; i++) {
			object[i] = init_object(&info.objs[i], GL_TRUE, GL_TRUE);
		}
		for (size_t i = 0; i < info.shaders->size; i++) {
			shader[i] = init_shader(info.shaders[i].vsrc.c_str(), info.shaders[i].fsrc.c_str());
		}

		for (size_t i = 0; i < info.strs[pronet::CHANCK_NATIVE]->size; i++) {
			structures[i] = init_structure(&info.strs[pronet::CHANCK_NATIVE][i].param,
				object[info.strs[pronet::CHANCK_NATIVE][i].buffer_object_index],
				shader[info.strs[pronet::CHANCK_NATIVE][i].shader_index],
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
		chanck.init(structures, &size_dir);
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline void loader<VBOLV, SHDLV>::load_playerinfo(const char* name, Player2vCreateInfo* const info)
	{
		reader.get_pnPlayer(name, info);
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline poolObject_shared_ptr<Object, VBOLV> loader<VBOLV, SHDLV>::init_object(ObjectInfo2v* const _info, bool _index_used, bool _texture_used) const
	{
		poolObject_shared_ptr<Object, VBOLV> object(&obj_pool);
		object->init(*dimentionSize, _info, _index_used, _texture_used);
		_info->reset();
		return object;
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline poolObject_shared_ptr<Shader, SHDLV> loader<VBOLV, SHDLV>::init_shader(const char* const _vsrc, const char* const _fsrc) const
	{
		poolObject_shared_ptr<Shader, SHDLV> shader(&shd_pool);
		shader->init(_vsrc, _fsrc);
		return shader;
	}

	template<std::size_t VBOLV, std::size_t SHDLV>
	inline poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> loader<VBOLV, SHDLV>::init_structure(const Structure2vParamCreateInfo *const strInfo, pronet::poolObject_shared_ptr<Object, VBOLV> object, pronet::poolObject_shared_ptr<Shader, SHDLV> shader, uint32_t tex_index) const
	{
		poolObject_shared_ptr<Structure2v<VBOLV, SHDLV>, strLv> structure(&str_pool);
		structure->init(strInfo, object, shader, tex_index);
		return structure;
	}
}