#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
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
        case UP:
            cameraPos.y += speed;
            break;
        case DOWN:
            cameraPos.y -= speed;
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
        // glm::vec3 position = glm::vec3(cameraPos.x, 0, cameraPos.z);
        // glm::vec3 target = glm::vec3(0,0,0);

        // glm::vec3 zaxis = glm::normalize(position - target);
        // // 3. Get positive right axis vector
        // glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(world_up), zaxis));
        // // 4. Calculate camera up vector
        // glm::vec3 yaxis = glm::cross(zaxis, xaxis);

        // // Create translation and rotation matrix
        // // In glm we access elements as mat[col][row] due to column-major layout
        // glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
        // translation[3][0] = -position.x;         // Fourth column, first row
        // translation[3][1] = -position.y;
        // translation[3][2] = -position.z;
        // glm::mat4 rotation = glm::mat4(1.0f);
        // rotation[0][0] = xaxis.x; // First column, first row
        // rotation[1][0] = xaxis.y;
        // rotation[2][0] = xaxis.z;
        // rotation[0][1] = yaxis.x; // First column, second row
        // rotation[1][1] = yaxis.y;
        // rotation[2][1] = yaxis.z;
        // rotation[0][2] = zaxis.x; // First column, third row
        // rotation[1][2] = zaxis.y;
        // rotation[2][2] = zaxis.z;

        // // Return lookAt matrix as combination of translation and rotation matrix
        // return rotation * translation;

        glm::mat4 view = glm::mat4(1);

        glm::vec3 r = cameraRight;
        glm::vec3 u = cameraUp;
        glm::vec3 f = cameraForward;

        glm::mat4 rotate = glm::mat4(
            -r.x, -u.x, -f.x, 0.0f,   // First column
            -r.y, -u.y, -f.y, 0.0f,   // Second column
            -r.z, -u.z, -f.z, 0.0f,   // Third column
            0.0f, 0.0f, 0.0f, 1.0f // Fourth column
        );

        glm::vec3 p = cameraPos;

        glm::mat4 translate = glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f, // First column
            0.0f, 1.0f, 0.0f, 0.0f, // Second column
            0.0f, 0.0f, 1.0f, 0.0f, // Third column
            -p.x, -p.y, -p.z, 1.0f  // Fourth column (translation)
        );

        view = rotate * translate;

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
