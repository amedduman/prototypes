#pragma once

#include "../include/GL/glew.h"
#define GL_SILENCE_DEPRECATION
#include "../include/GLFW/glfw3.h"
#include <string>
#include <vector>

GLFWwindow* init_window(int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSource(const std::string& relativePath);
unsigned int create_shader(GLenum shader_type, std::string relative_path_to_shader_source);
unsigned int create_shader_program(std::vector<unsigned int> shaders);
unsigned int create_model(const float* vertices, size_t vertices_size, const unsigned int* indices, size_t indices_size);