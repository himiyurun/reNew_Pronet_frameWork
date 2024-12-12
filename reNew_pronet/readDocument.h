#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/vec2.hpp>

#include "pnTlsf.h"

namespace pronet 
{
	//	vao �̏����i�[����
	struct vertexArrayInfo : public pnTlsf{
		vertexArrayInfo()
			: vertexcount(0)
			, verts(nullptr)
			, uv(nullptr)
			, indexcount(0)
			, index(nullptr)
			, shader_index(0xffff)
		{}

		size_t vertexcount;	//	���_�̐�
		glm::vec2 *verts;	//	���_����
		glm::vec2* uv;	//	UV���W
		size_t indexcount;		//	���_�C���f�b�N�X�̐�
		uint32_t *index;	//	���_�C���f�b�N�X
		uint8_t shader_index;	//	�V�F�[�_�[�̔ԍ�

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

		bool readFile(const char* name, std::unique_ptr<vertexArrayInfo[]>& info);

	private:

		bool type_correct(const std::string text);

		void getFromText(const std::string text, std::unique_ptr<vertexArrayInfo[]>& info);

		inline void getVerts(const char* script, std::unique_ptr<vertexArrayInfo[]>& info);

		inline void getIndex(const char* script, std::unique_ptr<vertexArrayInfo[]>& info);

		inline void getUv(const char* script, std::unique_ptr<vertexArrayInfo[]>& info);

		inline void getShader(const char* script, std::unique_ptr<vertexArrayInfo[]>& info);
	};

	void printVaoInfo(vertexArrayInfo *info);
}