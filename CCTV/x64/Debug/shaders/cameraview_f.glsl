#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable
out vec4 FragColor;
  
in vec2 TexCoords;

layout(location = 10) uniform vec2 screenSize;
layout(location = 11) uniform vec2 imageSize;

uniform sampler2D cameraTexture;


void main()
{ 
     vec2 texCoords = TexCoords;
     FragColor = texture(cameraTexture, texCoords);
}