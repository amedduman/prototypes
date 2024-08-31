#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

out vec3 Normal;
out vec3 FragPos; 
// out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // calculating model matrix and multiplying the normal to move it from model space to world space since 
    // of our light calculations are done in world space.
    Normal = mat3(transpose(inverse(model))) * aNormal;
    // TexCoords = aTexCoords;
    vs_out.texCoords = aTexCoords;
} 