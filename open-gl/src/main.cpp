#include "rendering_helper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main()
{
    GLFWwindow* window = init_window(800, 600);

    glfwSwapInterval(1);

    unsigned int vertexShader = create_shader(GL_VERTEX_SHADER, "src/my_first.vert");
    unsigned int fragmentShader = create_shader(GL_FRAGMENT_SHADER, "src/my_first.frag");
    unsigned int shaderProgram = create_shader_program({vertexShader, fragmentShader});

    float vertices[] = {
        // positions        // colors         // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO = create_model(vertices, sizeof(vertices), indices, sizeof(indices));

    unsigned int texture1 = create_texture("src/res/container.jpg", false);
    unsigned int texture2 = create_texture("src/res/awesomeface.png", true);

    glUseProgram(shaderProgram);
    
    // When you set a uniform to 0 or 1 in these lines, you're telling the shader: 
    // "The sampler uniform 'texture1' should use texture unit 0, and 'texture2' should use texture unit 1".
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
    // You're binding your actual texture objects to these same texture units.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    

    glBindVertexArray(VAO);

    float alpha = 1;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // std::cout << glfwGetTime() << "\n";
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            alpha += 0.01f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            alpha -= 0.01f;
        }
        if(alpha < 0 ) alpha = 0;
        if(alpha > 1) alpha = 1;
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(shaderProgram, "u_alpha"), alpha);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
