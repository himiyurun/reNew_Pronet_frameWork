#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

#include <glm/vec2.hpp>

#include "pnTlsf.h"
#include "Object.h"
#include "Shader.h"

namespace pronet 
{
	//	vao の情報を格納する
	struct vertexArrayInfo : public pnTlsf{
		vertexArrayInfo()
			: vertexcount(0)
			, verts(nullptr)
			, uv(nullptr)
			, indexcount(0)
			, index(nullptr)
			, shader_index(0xffff)
		{}

		size_t vertexcount;	//	頂点の数
		glm::vec2 *verts;	//	頂点属性
		glm::vec2* uv;	//	UV座標
		size_t indexcount;		//	頂点インデックスの数
		uint32_t *index;	//	頂点インデックス
		uint8_t shader_index;	//	シェーダーの番号

		~vertexArrayInfo() {
			if (verts) {
				delete_type<glm::vec2>(verts);
				std::cout << "deallocate" << std::endl;
			}
			if (uv) {
				delete_type<glm::vec2>(uv);
				std::cout << "deallocate" << std::endl;
			}
			if (index) {
				delete_type<uint32_t>(index);
				std::cout << "deallocate" << std::endl;
			}
		}
	};

	class PronetReadObject2v : public pnTlsf
	{
		const char* name;
		std::string data;
		std::string buf;
		std::ifstream file;
		std::istringstream iss;

		uint32_t points;
		size_t vaocount;
		size_t vertexcount;
		size_t indexcount;
		size_t nowVao;

	public:

		PronetReadObject2v();

		~PronetReadObject2v();

		virtual bool readFile(const char* name, std::unique_ptr<ObjectInfo2v[]>& info);

	private:

		bool type_correct(const std::string text);

		void getFromText(const std::string text, std::unique_ptr<ObjectInfo2v[]>& info);

		inline void getVerts(const char* script, std::unique_ptr<ObjectInfo2v[]>& info);

		inline void getIndex(const char* script, std::unique_ptr<ObjectInfo2v[]>& info);

		inline void getUv(const char* script, std::unique_ptr<ObjectInfo2v[]>& info);

		inline void getShader(const char* script, std::unique_ptr<ObjectInfo2v[]>& info);

		void fileclose(const char* log);
	};

	class readShaderMake {
		const char* name;

		std::string src;

		std::ifstream file;

		std::istringstream iss;

		std::string script;

		uint8_t points;

		uint8_t size;

	public:

		readShaderMake();

		~readShaderMake();

		virtual void readFile(const char* name, std::unique_ptr<ShaderMakeInfo[]>& info);

	private:

		inline bool type_correct(const char* script);

		inline void fileError(const char* MSG);

		inline void thMsg(const char* msg) const;

		inline void scriptFunc(const char* text, std::function<void()> func);

		void clear();
	};

	class PronetReadLoadFileList {
		const char* name;

		PronetReadObject2v objfile;
		readShaderMake shaderfile;

		uint8_t current;
		uint8_t points;
		size_t geometory;

		std::ifstream file;
		std::string line;
		std::string script;
		std::istringstream iss;

		uint16_t chanckSize;
		uint32_t shaderSize;
		uint32_t objectSize;

	public:
		struct PronetLoadChanckInfo {
			std::unique_ptr<std::unique_ptr<ShaderMakeInfo[]>[]> shaders;
			std::unique_ptr<std::unique_ptr<ObjectInfo2v[]>[]> objs;
		};

		PronetReadLoadFileList(const char* name, int* dimentionSize);

		~PronetReadLoadFileList();

		PronetLoadChanckInfo getLoadFile(uint32_t chanck_Index);

	private:

		inline PronetLoadChanckInfo getParam();

		inline bool type_correct(const char* script);

		inline void fileError(const char* MSG);

		inline void thMsg(const char* msg) const;

		inline void scriptFunc(const char* text, std::function<void()> func);

		void clear();

		void flash();
	};

	void printVaoInfo(ObjectInfo2v* info);
}