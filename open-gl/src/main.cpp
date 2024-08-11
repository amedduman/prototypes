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

//https://chatgpt.com/c/a550bdd5-110d-43e2-a38a-fb3f5e7ab695

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
        // second tri
        -0.9f, -0.4f, 0.0f, // bottom left
        -0.4f, 0.4f, 0.0f,  // top left
        0.1f, 0.4f, 0.0f, // top right
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        /*
        glDrawElements draws primitives using an index array, allowing you to reuse vertices.
        Instead of assuming that vertices are in sequential order, glDrawElements uses the indices to reference vertices in the vertex array, which can reduce the amount of data you need to send to the GPU.
        Parameters:

        mode: Specifies the type of primitive to draw (e.g., GL_TRIANGLES, GL_LINES, GL_POINTS).
        count: Specifies the number of elements (indices) to be rendered.
        type: Specifies the data type of the indices (e.g., GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT).
        indices: Specifies an offset into the index buffer, or a pointer to the location where the indices are stored.
        */
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
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

// what is the difference between glDrawArrays and glDrawElements
/*
glDrawArrays and glDrawElements are both functions used to draw primitives (like triangles, lines, or points) in OpenGL, but they differ in how they use the vertex data.
1. glDrawArrays

    Function Signature:

    cpp

    void glDrawArrays(GLenum mode, GLint first, GLsizei count);

Description:

    glDrawArrays draws primitives using the vertex data directly from the vertex array.
    It assumes that the vertex data is stored sequentially in the array, meaning that each vertex is used in the order it appears in the vertex buffer.
    Parameters:
        mode: Specifies the type of primitive to draw (e.g., GL_TRIANGLES, GL_LINES, GL_POINTS).
        first: Specifies the starting index in the array of vertices.
        count: Specifies the number of vertices to be rendered.

When to Use:

    Use glDrawArrays when you don’t need to reuse vertices and when the order of vertex processing is straightforward.
    This is typically used for simple shapes or when you don’t need to reduce the amount of vertex data.

Example:

    cpp

    glDrawArrays(GL_TRIANGLES, 0, 6);

        This would draw two triangles (6 vertices) from the vertex array, starting from the first vertex.

2. glDrawElements

    Function Signature:

    cpp

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);

Description:

    glDrawElements draws primitives using an index array, allowing you to reuse vertices.
    Instead of assuming that vertices are in sequential order, glDrawElements uses the indices to reference vertices in the vertex array, which can reduce the amount of data you need to send to the GPU.
    Parameters:
        mode: Specifies the type of primitive to draw (e.g., GL_TRIANGLES, GL_LINES, GL_POINTS).
        count: Specifies the number of elements (indices) to be rendered.
        type: Specifies the data type of the indices (e.g., GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT).
        indices: Specifies an offset into the index buffer, or a pointer to the location where the indices are stored.

When to Use:

    Use glDrawElements when you have shared vertices between primitives, such as when drawing complex shapes like cubes or other meshes where vertices are reused.
    It is more memory-efficient and can lead to better performance, especially with complex models.

Example:

cpp

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        This would draw two triangles using the 6 indices from the index buffer.

Key Differences:

    Data Usage:
        glDrawArrays: Uses vertices in the order they appear in the vertex buffer.
        glDrawElements: Uses an index array to specify the order of vertices, which allows for vertex reuse.

    Efficiency:
        glDrawArrays: Can be less efficient for complex shapes as it might require duplicating vertex data.
        glDrawElements: More efficient for complex shapes due to vertex reuse, reducing the amount of vertex data needed.

    Flexibility:
        glDrawArrays: Simpler to use for basic shapes where vertices are not shared.
        glDrawElements: More flexible and powerful for complex shapes and meshes with shared vertices.

In summary, glDrawArrays is straightforward but can be less efficient for complex models, while glDrawElements is more flexible and efficient when vertices are shared across multiple primitives.
*/