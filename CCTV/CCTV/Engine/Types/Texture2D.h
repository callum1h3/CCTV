#pragma once

#include "../Resources.h"
#include "../OpenGL.h"

namespace Engine
{
    class Texture2D : public ResourceObject {
    public:
        void Load(std::string fullpath);
        void Create(int _width, int _height, GLint data_type);
        void Dispose();

        void Set(unsigned char* data);

        int GetWidth();
        int GetHeight();
        bool IsValid();
    private:
        int width, height;
        GLint dataType;
        unsigned int buffer;
        bool isValid = false;
    };

}