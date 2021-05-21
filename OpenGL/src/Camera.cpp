#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
    //return myLookAt();
}

glm::mat4 Camera::myLookAt() {
    glm::mat4 matrix2 = glm::translate(glm::mat4(1.0), -Position);
    glm::vec3 target = Position + Front;
    glm::vec3 d = glm::normalize(Position - target); // pos - (pos + front) = -front направление НА камеру, а не от камеры, поэтому -Front
    glm::vec3 r = glm::normalize(glm::cross(WorldUp, d));
    glm::vec3 u = glm::normalize(glm::cross(d, r));
    glm::mat4 matrix1 = glm::mat4(glm::vec4(r.x, u.x, d.x, 0.0f), glm::vec4(r.y, u.y, d.y, 0.0f), 
        glm::vec4(r.z, u.z, d.z, 0.0f), glm::vec4(glm::vec3(0.0f), 1.0f));
    return matrix1 * matrix2;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        Position += Front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        Position -= Front * velocity;
    if (direction == Camera_Movement::LEFT)
        Position -= Right * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position += Right * velocity;
    //Position.y = 0.0f; // мы и так остаемся на плоскости xz
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Убеждаемся, что когда тангаж выходит за пределы обзора, экран не переворачивается
    if (constrainPitch)
        Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
    
    // Обновляем значения вектора-прямо, вектора-вправо и вектора-вверх, используя обновленные значения углов Эйлера
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset; // yoffset may be < 0
    Zoom = glm::clamp(Zoom, 1.0f, 45.0f);
}
void Camera::updateCameraVectors()
{
    // Вычисляем новый вектор-прямо
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Также пересчитываем вектор-вправо и вектор-вверх
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Нормализуем векторы, потому что их длина становится стремится к 0 тем больше, чем больше вы смотрите вверх или вниз, что приводит к более медленному движению.
    Up = glm::normalize(glm::cross(Right, Front));
}