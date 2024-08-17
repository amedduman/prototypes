#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct camera
{
    bool firstMouse = true;
    float lastX = 400, lastY = 300;

    float fov = 45;
    float yaw = -90.0;
    float pitch = 0;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 direction;

    const float cameraSpeed = 2.5f; // adjust accordingly

    void handle_keyboard_input(Camera_Movement input_dir, float delta_time)
    {
        float speed = cameraSpeed * delta_time;

        // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //     cameraPos += speed * cameraFront;
        // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //     cameraPos -= speed * cameraFront;
        // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
        // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;

        switch (input_dir)
        {
        case FORWARD:
            cameraPos += speed * cameraFront;
            break;
        case BACKWARD:
            cameraPos -= speed * cameraFront;
            break;
        case LEFT:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        case RIGHT:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        default:
            break;
        }

        // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        // direction.y = sin(glm::radians(pitch));
        // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        // cameraFront = glm::normalize(direction);
    }

    void look_around(float xpos, float ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    void zoom(float yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
};
