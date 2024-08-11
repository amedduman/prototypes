#include "../include/GL/glew.h"
#define GL_SILENCE_DEPRECATION
#include "../include/GLFW/glfw3.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;

GLFWwindow* init_window(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSource(const std::string& relativePath);
void create_shader(unsigned int& shader_id, GLenum shader_type, std::string relative_path_to_shader_source);
void create_shader_program(unsigned int& shader_prog_id, std::vector<unsigned int> shaders);

int main()
{
    GLFWwindow* window = init_window(800, 600);

    unsigned int vertexShader;
    create_shader(vertexShader, GL_VERTEX_SHADER, "src/my_first.vert");

    unsigned int fragmentShader;
    create_shader(fragmentShader, GL_FRAGMENT_SHADER, "src/my_first.frag");

    unsigned int shaderProgram;
    create_shader_program(shaderProgram, {vertexShader, fragmentShader});

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    /*
    VAO (Vertex Array Object): This is a container that
    stores the state associated with the vertex attributes and the vertex buffer object (VBO).

    glGenVertexArrays(1, &VAO); generates a VAO and stores its ID in VAO.
    glBindVertexArray(VAO); binds the VAO, making it the current VAO that OpenGL will use for subsequent operations.
    */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /*
    VBO (Vertex Buffer Object): This is used to store the vertex data in GPU memory.
    glGenBuffers(1, &VBO); generates a VBO and stores its ID in VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); binds the VBO as the current GL_ARRAY_BUFFER, which is a target buffer type for vertex data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); uploads the vertex data to the VBO.
        GL_STATIC_DRAW indicates that the data will not change frequently and is meant for drawing.
    */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    EBO (Element Buffer Object): This is used to store the index data in GPU memory.

    glGenBuffers(1, &EBO); generates an EBO and stores its ID in EBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); binds the EBO as the current GL_ELEMENT_ARRAY_BUFFER, which is a target buffer type for index data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); uploads the index data to the EBO
    */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*
    glVertexAttribPointer: This function specifies the layout of the vertex data in the VBO.
    0 is the index of the vertex attribute. It corresponds to the layout location in your vertex shader.
    3 is the number of components per vertex attribute (x, y, z).
    GL_FLOAT indicates that the data type of each component is a float.
    GL_FALSE means the data should not be normalized.
    3 * sizeof(float) is the stride, which is the byte offset between consecutive vertex attributes.
    (void*)0 is the offset of the first component of the attribute within the vertex data.
    glEnableVertexAttribArray(0): This enables the vertex attribute array at index 0.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        /*
        glUseProgram(shaderProgram); activates the shader program.
        glBindVertexArray(VAO); binds the VAO containing the vertex and index data.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); draws the triangles using the index data.

        GL_TRIANGLES specifies that the data should be interpreted as triangles.
        6 is the number of elements to draw.
        GL_UNSIGNED_INT specifies the data type of the indices.
        0 is the offset in the EBO where the indices start.

        glBindVertexArray(0); unbinds the VAO (optional but good practice).
        */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    cout << width << ", " << height << endl; 
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
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

void create_shader(unsigned int& shader_id, GLenum shader_type, std::string relative_path_to_shader_source)
{
    shader_id = glCreateShader(shader_type);
    std::string shader_source = loadShaderSource(relative_path_to_shader_source);
    const char* source_ptr = shader_source.c_str();
    /*
    This function associates the shader source code with the shader object. The parameters are:
    1-The shader object (fragmentShader)
    2-The number of strings in the source code array (1 in this case)
    3-A pointer to the array of source code strings (&fragmentShaderSource)
    4-An array of string lengths (NULL here, which means the strings are null-terminated)
    */
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
}

void create_shader_program(unsigned int& shader_prog_id, std::vector<unsigned int> shaders)
{
    shader_prog_id = glCreateProgram();

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
}
