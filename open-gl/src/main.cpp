#include "Model.h"
#include "camera.h"
#include "rendering_helper.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static camera cam;

void processInput(GLFWwindow* window, float delta_time);
void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos);
void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset);

int main()
{
    GLFWwindow* window = init_window(SCR_WIDTH, SCR_HEIGHT);

    glfwSwapInterval(1);

    unsigned int vertexShader = create_shader(GL_VERTEX_SHADER, "src/my_first.vert");
    unsigned int fragmentShader = create_shader(GL_FRAGMENT_SHADER, "src/my_first.frag");
    unsigned int shaderProgram = create_shader_program({vertexShader, fragmentShader});

    // unsigned int light_source_v = create_shader(GL_VERTEX_SHADER, "src/my_first.vert");
    // unsigned int light_source_s = create_shader(GL_FRAGMENT_SHADER, "src/ligth_source.frag");
    // unsigned int light_source_shader = create_shader_program({light_source_v, light_source_s});

    // unsigned int diffuseMap = create_texture("src/res/container2.png", true);
    // unsigned int specularMap = create_texture("src/res/container2_specular.png", true);

    stbi_set_flip_vertically_on_load(true);

    std::string path = std::filesystem::absolute("src/res/backpack/backpack.obj").string();
    char* path_c_str = new char[path.length() + 1];
    std::strcpy(path_c_str, path.c_str());
    Model backpack(path_c_str);

    glEnable(GL_DEPTH_TEST);

    float delta_time = 0.0f;
    float lastFrame = 0.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        { // per-frame time logic
            float currentFrame = static_cast<float>(glfwGetTime());
            delta_time = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        { // input
            processInput(window, delta_time);
        }

        { // render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cam.cameraPos, cam.cameraPos + cam.cameraForward, cam.cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), cam.cameraPos.x, cam.cameraPos.y, cam.cameraPos.z);

        { // material
            glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 1.0f, 0.5f, 0.31f);
            glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 1.0f, 0.5f, 0.31f);
            glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.5f);
            glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);

            // directional light
            glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), 0.2f, 0.5f, 0.2f);
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
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].position"), 1, 1, 1);
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 1.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09f);
            glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032f);
        }

        backpack.Draw(shaderProgram);

        //--------------------------------------------------------------------------------

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
