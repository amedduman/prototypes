#version 330 core

in vec3 vPos;
in vec3 vColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(vColor, 1.0);
}