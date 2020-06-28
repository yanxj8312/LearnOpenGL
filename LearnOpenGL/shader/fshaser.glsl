#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D picTexture;
uniform sampler2D picTexture1;

void main()
{
    FragColor = mix(texture(picTexture,TexCoord),texture(picTexture1,TexCoord),0.5);
}