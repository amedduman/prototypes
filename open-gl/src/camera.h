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
    float lastX = 400;
    float lastY = 300;

    const float cameraSpeed = 2.5f; // adjust accordingly
    const float sensitivity = 0.1f;

    float fov = 45; 

    float yaw = -90.0;
    float pitch = 0;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);


    void handle_keyboard_input(Camera_Movement input_dir, float delta_time)
    {
        float speed = cameraSpeed * delta_time;

        switch (input_dir)
        {
        case FORWARD:
            cameraPos += cameraForward * speed;
            break;
        case BACKWARD:
            cameraPos -= cameraForward * speed;
            break;
        case LEFT:
            cameraPos -= cameraRight * speed;
            break;
        case RIGHT:
            cameraPos += cameraRight * speed;
            break;
        default:
            break;
        }
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

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        update_cam_vectors();
    }

    void zoom(float yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    // what is direction and cameraFront?
    // TODO: refactor camera code and then implement this function.
    glm::mat4 get_view_mat()
    {
        glm::mat4 view = glm::mat4(1);

        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraForward, glm::vec3(0, 1, 0)));

        glm::vec3 r = cameraRight;
        glm::vec3 u = glm::normalize(glm::cross(cameraRight, cameraForward));
        glm::vec3 d = cameraForward;

        glm::mat4 m1 = glm::mat4(
            r.x, r.y, r.z, 0.0f,   // First column
            u.x, u.y, u.z, 0.0f,   // Second column
            d.x, d.y, d.z, 0.0f,   // Third column
            0.0f, 0.0f, 0.0f, 1.0f // Fourth column
        );

        glm::vec3 p = cameraPos;

        glm::mat4 m2 = glm::mat4(
            1.0f, 0.0f, 0.0f, -p.x, // First column
            0.0f, 1.0f, 0.0f, -p.y, // Second column
            0.0f, 0.0f, 1.0f, -p.z, // Third column
            0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
        );

        view = m1 * m2;

        return view;
    }

private:
    const glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    void update_cam_vectors()
    {
        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraForward = glm::normalize(direction);
        cameraRight = glm::normalize(glm::cross(cameraForward, world_up));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraForward));
    }
};
