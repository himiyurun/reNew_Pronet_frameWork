#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

#include <glm/vec2.hpp>

#include "ObjectPoolArray.h"
#include "Object.h"
#include "Shader.h"

namespace pronet 
{
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

		virtual bool readFile(const char* name, ObjectInfo2v* info, pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool);

	private:

		bool type_correct(const std::string text);

		void getFromText(const std::string text, ObjectInfo2v* info, pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool);

		inline void getVerts(const char* script, ObjectInfo2v* info, pronet::ObjectPool_Array<glm::vec2>* vertsPool);

		inline void getIndex(const char* script, ObjectInfo2v* info, pronet::ObjectPool_Array<uint32_t>* indexPool);

		inline void getUv(const char* script, ObjectInfo2v* info);

		inline void getShader(const char* script, ObjectInfo2v* info);

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
			std::unique_ptr<ShaderMakeInfo[]> shaders;
			std::unique_ptr<ObjectInfo2v[]> objs;
		};

		PronetReadLoadFileList(const char* name, int* dimentionSize);

		~PronetReadLoadFileList();

		PronetLoadChanckInfo getLoadFile(uint32_t chanck_Index, pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool);

	private:

		inline PronetLoadChanckInfo getParam(pronet::ObjectPool_Array<glm::vec2>* vertsPool, pronet::ObjectPool_Array<uint32_t>* indexPool);

		inline bool type_correct(const char* script);

		inline void fileError(const char* MSG);

		inline void thMsg(const char* msg) const;

		inline void scriptFunc(const char* text, std::function<void()> func);

		void clear();

		void flash();
	};

	void printVaoInfo(ObjectInfo2v* info);
}