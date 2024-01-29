#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Engine;

void Texture2D::Load(std::string fullpath)
{
	int _width, _height, nrChannels;
	unsigned char* data = stbi_load(fullpath.c_str(), &_width, &_height, &nrChannels, 0);

	GLint data_type = GL_RGB ;
	if (nrChannels == 4)
		data_type = GL_RGBA;

	std::cout << nrChannels;
	 
	Create(_width, _height, data_type);
	Set(data);
}

void Texture2D::Create(int _width, int _height, GLint data_type)
{
	width = _width;
	height = _height;
	dataType = data_type;

	glGenTextures(1, &buffer);
	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, data_type, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	isValid = true;
}

void Texture2D::Set(unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, dataType, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Dispose()
{
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