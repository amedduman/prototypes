#include "Model.h"
#include "camera.h"
#include "rendering_helper.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static camera cam;

void processInput(GLFWwindow* window, float delta_time);
void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos);
void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset);

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(const std::string& path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 1;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /*
            Note that when sampling textures at their borders, OpenGL interpolates the border values with the next repeated value of the texture (because we set its wrapping parameters to GL_REPEAT by default). This is usually okay, but since we're using transparent values, the top of the texture image gets its transparent value interpolated with the bottom border's solid color value. The result is then a slightly semi-transparent colored border you may see wrapped around your textured quad. To prevent this, set the texture wrapping method to GL_CLAMP_TO_EDGE whenever you use alpha textures that you don't want to repeat:
        */
        if (format == GL_RGBA)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    stbi_set_flip_vertically_on_load(false); // for some reasen the skybox is upside down. when this is true.

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);

    return textureID;
}

int main()
{
    GLFWwindow* window = init_window(SCR_WIDTH, SCR_HEIGHT);

    glfwSwapInterval(1);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float delta_time = 0.0f;
    float lastFrame = 0.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int vertexShader = create_shader(GL_VERTEX_SHADER, "src/my_first.vert");
    unsigned int fragmentShader = create_shader(GL_FRAGMENT_SHADER, "src/my_first.frag");
    unsigned int shaderProgram = create_shader_program({vertexShader, fragmentShader});

    stbi_set_flip_vertically_on_load(true);

    unsigned int metal_tex = loadTexture("src/res/textures/wood.png");

#pragma region plane

    float points[] = {
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
    };
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

#pragma endregion

    Model backpack(std::filesystem::absolute("src/res/plane.obj").string());

    while (!glfwWindowShouldClose(window))
    {
        { // per-frame time logic
            float currentFrame = static_cast<float>(glfwGetTime());
            delta_time = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        processInput(window, delta_time);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraForward, cam.cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindTexture(GL_TEXTURE_2D, metal_tex);

        glm::vec3 point_light_pos(0.0f, 0.3f, 0.0f);
        glm::vec3 point_light_color(1.0f, 1.0f, 1.0f);
        { // lighting
            glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &cam.cameraPos[0]);
            glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);

            // directional light
            glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), 0.0f, -10.0f, 10.0f);
            glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
            glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

            // spot light
            glUniform3fv(glGetUniformLocation(shaderProgram, "spotLight.position"), 1, &cam.cameraPos[0]);
            glUniform3fv(glGetUniformLocation(shaderProgram, "spotLight.direction"), 1, &cam.cameraForward[0]);
            glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
            glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
            glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.2f, 0.2f, 0.2f);
            glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), 0.09f);
            glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), 0.032f);

            // point light
            glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].position"), 1, &point_light_pos[0]);
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 0.5f, 0.5f, 0.5f);
            glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 1, &point_light_color[0]);
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032f);
        }

        backpack.Draw(shaderProgram);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.handle_keyboard_input(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.handle_keyboard_input(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.handle_keyboard_input(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.handle_keyboard_input(RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.handle_keyboard_input(UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.handle_keyboard_input(DOWN, delta_time);
}

void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos)
{
    cam.look_around(xpos, ypos);
}

void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset)
{
    cam.zoom(yoffset);
}