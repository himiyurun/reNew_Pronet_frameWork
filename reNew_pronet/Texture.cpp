#include "Texture.h"

Texture::Texture(const char* _imgname)
	: tex(0), is_used(false)
{
	if (_imgname)
		init(_imgname);
}

Texture::~Texture()
{
	if (is_used)
		reset();
}

void Texture::init(const char* _imgname)
{
	if (is_used)
		throw std::runtime_error("this object is already used!! : Texture.init(const char*)");

	pronet::readBMP image(_imgname);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glad_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (image->biBitCount == 0x18) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->biWidth, image->biHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, image.pixcel());
	}
	if (image->biBitCount == 0x24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->biWidth, image->biHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.pixcel());
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	is_used = true;
}

void Texture::reset()
{
	glDeleteTextures(1, &tex);
	is_used = false;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Texture::active() const
{
	glActiveTexture(GL_TEXTURE0);
}

void Texture::exe(GLuint _id) const
{
	if (_id == 0xffffffff)
		throw std::runtime_error("this shader don't use texture!! : Texture.exe(GLuint)");
	glUniform1i(_id, 0);
	active();
	bind();
}
