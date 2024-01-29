#version 330 core
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout (std140, binding = 1) uniform ui_storage
{
    mat4 ui_projection;

}; 

void main()
{
    gl_Position = ui_projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  