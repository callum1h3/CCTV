#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable
out vec4 FragColor;
  
in vec2 TexCoords;

layout(location = 10) uniform vec2 screenSize;
layout(location = 11) uniform vec2 screenPosition;
layout(location = 12) uniform float screenScale;
layout(location = 13) uniform float time;

uniform sampler2D screenTexture;
uniform sampler2D constructionTexture;

void main()
{ 
    float sizeOfCell = 32;
    float xCoords = screenSize.x / sizeOfCell;
    float yCoords = screenSize.y / sizeOfCell;

    float sizeOfBordersX = sizeOfCell / screenSize.x;
    float sizeOfBordersY = sizeOfCell / screenSize.y;

    float screenX = screenPosition.x / screenSize.x;
    float screenY = screenPosition.y / screenSize.y;

    vec2 screenPosToUV = vec2(screenX, screenY);
    vec2 texCoords = (TexCoords * vec2(xCoords, yCoords)) + screenPosToUV;
    vec2 finalCoords = (texCoords * screenScale);
    
    if (TexCoords.x < sizeOfBordersX || TexCoords.x > (1 - sizeOfBordersX) || TexCoords.y < sizeOfBordersY || TexCoords.y > (1 - sizeOfBordersY))
    	FragColor = texture(constructionTexture, texCoords + vec2(0, time));
    else
        FragColor = texture(screenTexture, finalCoords);
}