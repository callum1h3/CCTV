#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Engine;

void Texture2D::Load(std::string fullpath)
{
	stbi_set_flip_vertically_on_load(1);

	int _width, _height, nrChannels;
	
	unsigned char* data = stbi_load(fullpath.c_str(), &_width, &_height, &nrChannels, 0);

	if (data == nullptr)
	{
		std::cout << "Failed to load image \n";
	}

	GLenum format{};

	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	Create(_width, _height, format);
	Set(data);
	SetFilter(GL_LINEAR);

	stbi_image_free(data);
}

void Texture2D::Create(int _width, int _height, GLint data_type)
{
	width = _width;
	height = _height;
	dataType = data_type;

	glGenTextures(1, &buffer);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, data_type, width, height, 0, data_type, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	isValid = true;
}

void Texture2D::Set(unsigned char* data)
{
	__data = data;
	glBindTexture(GL_TEXTURE_2D, buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, dataType, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenerateMipmap(buffer);
}

unsigned char* Texture2D::GetData()
{
	return __data;
}

void Texture2D::SetFilter(GLint param)
{
	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Dispose()
{
	if (!isValid)
		return;

	if (__data != nullptr)
		delete __data;

	isValid = false;
	glDeleteTextures(1, &buffer);
}

unsigned int Texture2D::GetBuffer()
{
	return buffer;
}

int Texture2D::GetWidth()
{
	return width;
}

int Texture2D::GetHeight()
{
	return height;
}

bool Texture2D::IsValid()
{
	return isValid;
}

void Texture2D::SetFlipped(bool s)
{
	isFlipped = s;
}

bool Texture2D::IsFlipped()
{
	return isFlipped;
}
