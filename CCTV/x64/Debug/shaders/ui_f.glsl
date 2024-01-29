#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 4) uniform vec4 color;

layout (location = 0) out vec4 gPosition;
in vec2 TexCoords;

uniform sampler2D iconTexture;

void main()
{
    vec4 textureColor = texture(iconTexture, TexCoords);
    gPosition = textureColor * color;
} 