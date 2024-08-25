#pragma once

#include "GL/glew.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <vector>

using std::vector;
using std::string;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture
{
    unsigned int id;
    std::string type; // type e.g. a diffuse or specular texture
};

class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh() = delete;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(unsigned int shader);
    ~Mesh() = default;

private:
    //  render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

// impl -----------------------------------------------------------------------------------

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(unsigned int shader)
{
    /*
        this solution requires shaders to have:

        struct Material {
        sampler2D texture_diffuse1;
        sampler2D texture_diffuse2;
        .
        .
        .
        sampler2D texture_specular1;
        sampler2D texture_specular2;
        .
        .
        .
        };
        uniform Material material;
    */
    unsigned int diffuse_counter = 1;
    unsigned int specular_counter = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        string number;
        string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuse_counter);
        else if (name == "texture_specular")
            number = std::to_string(specular_counter);

        string uniform_name = "material." + name + number;

        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        glUniform1i(glGetUniformLocation(shader, uniform_name.c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);

        diffuse_counter++;
        specular_counter++;
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
