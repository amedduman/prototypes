#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include "rendering_helper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::cerr;
using std::cout;
using std::endl;

GLFWwindow* init_window(int width, int height)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return nullptr;
    }

    cout << glGetString(GL_VERSION) << endl;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    return window;
}

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
{
    cout << width << ", " << height << endl;
    glViewport(0, 0, width, height);
}

std::string loadShaderSource(const std::string& relativePath)
{
    // Get the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();

    // Construct the full path
    std::filesystem::path fullPath = currentPath / relativePath;

    // Ensure the file exists
    if (!std::filesystem::exists(fullPath))
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_FOUND: " + fullPath.string());
    }

    std::string shaderCode;
    std::ifstream shaderFile;

    // Ensure ifstream objects can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open file
        shaderFile.open(fullPath);
        std::stringstream shaderStream;

        // Read file's buffer contents into stream
        shaderStream << shaderFile.rdbuf();

        // Close file handler
        shaderFile.close();

        // Convert stream into string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + fullPath.string());
    }

    return shaderCode;
}

unsigned int create_shader(GLenum shader_type, std::string relative_path_to_shader_source)
{
    unsigned int shader_id = glCreateShader(shader_type);
    std::string shader_source = loadShaderSource(relative_path_to_shader_source);
    const char* source_ptr = shader_source.c_str();

    glShaderSource(shader_id, 1, &source_ptr, NULL);
    glCompileShader(shader_id);

    { // error handling code
        int success;
        char infoLog[512];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }
    return shader_id;
}

unsigned int create_shader_program(std::vector<unsigned int> shaders)
{
    unsigned int shader_prog_id = glCreateProgram();

    for (auto& s : shaders)
    {
        glAttachShader(shader_prog_id, s);
    }

    glLinkProgram(shader_prog_id);

    { // error handling code
        int success;
        char infoLog[512];
        glGetProgramiv(shader_prog_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader_prog_id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
    }

    for (auto& s : shaders)
    {
        glDeleteShader(s);
    }

    return shader_prog_id;
}

unsigned int create_model(const float* vertices, size_t vertices_size, const unsigned int* indices, size_t indices_size)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

unsigned int create_texture(const std::string& relativePath, bool has_a_channel)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(relativePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = has_a_channel ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << relativePath << std::endl;
    }
    stbi_image_free(data);

    return texture_id;
}
