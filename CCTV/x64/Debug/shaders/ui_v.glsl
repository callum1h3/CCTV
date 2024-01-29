#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 2) uniform vec2 position;
layout(location = 3) uniform vec2 size;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


layout (std140, binding = 1) uniform ui_storage
{
    mat4 ui_projection;
}; 

void main()
{
    gl_Position = ui_projection * vec4(position.x + (aPos.x * size.x), position.y + (aPos.y * size.y), aPos.z, 1.0);
    TexCoords = aTexCoords;
}