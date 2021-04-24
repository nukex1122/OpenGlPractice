#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP, 
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float fov;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, float pitch = PITCH) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), 
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(ZOOM){
        cameraPos = position;
        worldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, 
        float upX, float upY, float upZ, 
        float yaw, float pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), 
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(ZOOM)
    {
        cameraPos = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            cameraPos += cameraFront * velocity;
        if (direction == BACKWARD)
            cameraPos -= cameraFront * velocity;
        if (direction == LEFT)
            cameraPos -= cameraRight * velocity;
        if (direction == RIGHT)
            cameraPos += cameraRight * velocity;
        if (direction == UP)
            cameraPos += worldUp * velocity;
        if (direction == DOWN)
            cameraPos -= worldUp * velocity;

    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();

        
    }

    void ProcessMouseScroll(float yoffset) {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0;
        if (fov > 45.0f)
            fov = 45.0;
    }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        direction.y = sin(glm::radians(Pitch));
        direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        cameraFront = glm::normalize(direction);
        
        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    }

};
#endif