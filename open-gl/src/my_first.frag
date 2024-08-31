#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float     shininess;
}; 
uniform Material material;

in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    FragColor = texture(material.texture_diffuse1, TexCoords);   
}