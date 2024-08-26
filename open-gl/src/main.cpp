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

    stbi_set_flip_vertically_on_load(true);

    std::string path = std::filesystem::absolute("src/res/backpack/backpack.obj").string();
    Model backpack(path);

    glEnable(GL_DEPTH_TEST);

    float delta_time = 0.0f;
    float lastFrame = 0.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

#pragma region lamp debug shape

    float vertices[] = {
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,

        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,

        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,

        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,

        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,

        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int light_source_v = create_shader(GL_VERTEX_SHADER, "src/my_first.vert");
    unsigned int light_source_s = create_shader(GL_FRAGMENT_SHADER, "src/ligth_source.frag");
    unsigned int light_source_shader = create_shader_program({light_source_v, light_source_s});

#pragma endregion

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

        glm::vec3 point_light_pos(0.0f, 0.0f, 10.0f);
        glm::vec3 point_light_color(1.0f, 0.0f, 0.0f);

        { // lighting
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

        //--------------------------------------------------------------------------------

        { // draw light source
            glUseProgram(light_source_shader);

            glUniform3fv(glGetUniformLocation(light_source_shader, "light_color"), 1, &point_light_color[0]);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, point_light_pos);
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(glGetUniformLocation(light_source_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(light_source_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(light_source_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

// Place your key bindings in this file to override the defaultsauto[]
[
    // run task
    {
        "key" : "[IntlBackslash]",
        "command" : "workbench.action.tasks.runTask",
        "args" : "Run"
    },
    // left
    {
        "key" : "cmd+l",
        "command" : "cursorRight",
        "when" : "textInputFocus"
    },
    // right
    {
        "key" : "cmd+h",
        "command" : "cursorLeft",
        "when" : "textInputFocus"
    },
    // down
    {
        "key" : "cmd+j",
        "command" : "cursorDown",
        "when" : "textInputFocus && !suggestWidgetVisible"
    },
    // down (suggestionWidget)
    {
        "key" : "cmd+j",
        "command" : "selectNextSuggestion",
        "when" : "suggestWidgetVisible"
    },
    // up
    {
        "key" : "cmd+k",
        "command" : "cursorUp",
        "when" : "textInputFocus && !suggestWidgetVisible"
    },
    // up (suggestionWidget)
    {
        "key" : "cmd+k",
        "command" : "selectPrevSuggestion",
        "when" : "suggestWidgetVisible"
    },
    // put line below
    {
        "key" : "cmd+o",
        "command" : "editor.action.insertLineAfter",
        "when" : "textInputFocus && !suggestWidgetVisible"
    },
    // insert suggestion (auto-complete)
    {
        "key" : "cmd+o",
        "command" : "acceptSelectedSuggestion",
        "when" : "suggestWidgetVisible"
    },
    // set capslock to control key
    {
        "key" : "Capslock",
        "command" : "",
        "when" : "editorFocus"
    },
    // copy
    {
        "key" : "cmd+y",
        "command" : "execCopy",
        "when" : "editorFocus"
    },
    // cut
    {
        "key" : "cmd+u",
        "command" : "execCut",
        "when" : "editorFocus"
    },
    // paste
    {
        "key" : "alt+p",
        "command" : "execPaste",
        "when" : "editorFocus"
    },
    // backspace
    {
        "key" : "cmd+i",
        "command" : "deleteLeft",
        "when" : "editorFocus"
    },
    // move end of line
    {
        "key" : "cmd+]",
        "command" : "cursorEnd",
        "when" : "editorFocus"
    },
    // move start of line
    {
        "key" : "cmd+[",
        "command" : "cursorHome",
        "when" : "editorFocus"
    },
    // move end of line (selected)
    {
        "key" : "ctrl+]",
        "command" : "cursorEndSelect",
        "when" : "editorFocus"
    },
    // move start of line (selected)
    {
        "key" : "ctrl+[",
        "command" : "cursorHomeSelect",
        "when" : "editorFocus"
    },
    // page down
    {
        "key" : "alt+j",
        "command" : "cursorPageDown",
        "when" : "editorFocus"
    },
    // page up
    {
        "key" : "alt+k",
        "command" : "cursorPageUp",
        "when" : "editorFocus"
    },
    // select down
    {
        "key" : "ctrl+j",
        "command" : "cursorDownSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // select up
    {
        "key" : "ctrl+k",
        "command" : "cursorUpSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // select left
    {
        "key" : "ctrl+l",
        "command" : "cursorRightSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // select right
    {
        "key" : "ctrl+h",
        "command" : "cursorLeftSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // left by work
    {
        "key" : "alt+h",
        "command" : "cursorWordEndLeft",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // right by word
    {
        "key" : "alt+l",
        "command" : "cursorWordEndRight",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // left by word (selected)
    {
        "key" : "alt+ctrl+h",
        "command" : "cursorWordEndLeftSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // right by word (selected)
    {
        "key" : "alt+ctrl+l",
        "command" : "cursorWordEndRightSelect",
        "when" : "editorFocus && !suggestWidgetVisible"
    },
    // escape
    {
        "key" : "cmd+i",
        "command" : "hideSuggestWidget",
        "when" : "editorFocus && suggestWidgetVisible"
    },
    // next tab
    {
        "key" : "cmd+up",
        "command" : "workbench.action.nextEditor",
        "when" : "editorFocus && !suggestWidgetVisible",
    },
    // previous tab
    {
        "key" : "cmd+down",
        "command" : "workbench.action.previousEditor",
        "when" : "editorFocus && !suggestWidgetVisible",
    },
]
