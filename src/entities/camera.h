#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Kredo
{

class Camera
{
public:
    enum MoveDirection
    {
        ForwardDirection,
        BackwardDirection,
        LeftDirection,
        RightDirection,
        UpDirection,
        DownDirection
    };

public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

    glm::mat4 GetViewMatrix() const;
    float GetFov() const;

    void Move(MoveDirection direction, float delta);
    void Rotate(float x, float y);

private:
    void Update();

private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;
    float _fov;

    float _moveSpeed;
    float _rotateSensitivity;
};

}