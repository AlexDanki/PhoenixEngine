#version 330 core

in vec2 TexCoord;

uniform sampler2D textTexture;

out vec4 FragColor;

void main()
{
    float alpha = texture(textTexture, TexCoord).r;
    FragColor = vec4(1,1,1,alpha);
}