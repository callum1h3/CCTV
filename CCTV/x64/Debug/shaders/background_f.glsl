#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
out vec4 FragColor;
  
in vec2 TexCoords;

layout(location = 10) uniform vec2 screenSize;
layout(location = 11) uniform vec2 screenPosition;
layout(location = 12) uniform float screenScale;

uniform sampler2D screenTexture;

void main()
{ 
    float sizeOfCell = 32;
    float xCoords = screenSize.x / sizeOfCell;
    float yCoords = screenSize.y / sizeOfCell;
    vec2 screenPosToUV = (screenPosition / screenSize) * screenScale;

    FragColor = texture(screenTexture, (TexCoords * vec2(xCoords, yCoords) * screenScale) + screenPosToUV);
}