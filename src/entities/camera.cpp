#include "camera.h"

#include <wx/log.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Kredo
{

Camera::Camera(const glm::vec3& position)
    : _position(position)
    , _front(0.0f, 0.0f, -1.0f)
    , _right(1.0f, 0.0f, 0.0f)
    , _up(0.0f, 1.0f, 0.0f)
    , _worldUp(0.0f, 1.0f, 0.0f)
    , _yaw(-90.0f)
    , _pitch(0.0f)
    , _moveSpeed(2.5f)
    , _rotateSensitivity(0.05f)
    , _viewport(0, 0)
    , _fov(60.0f)
    , _nearPlane(0.1f)
    , _farPlane(100.0f)
{
    Update();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(_fov), float(_viewport.GetWidth()) / float(_viewport.GetHeight()), _nearPlane, _farPlane);
}

void Camera::SetViewport(int width, int height)
{
    _viewport.Set(width, height);
}

wxSize Camera::GetViewport() const
{
    return _viewport;
}

void Camera::Move(MoveDirection direction, float delta)
{
    const auto velocity = delta * _moveSpeed;

    switch (direction)
    {
    case ForwardDirection:
        _position += _front * velocity;
        break;

    case BackwardDirection:
        _position -= _front * velocity;
        break;

    case LeftDirection:
        _position -= _right * velocity;
        break;

    case RightDirection:
        _position += _right * velocity;
        break;

    case UpDirection:
        _position += _worldUp * velocity;
        break;

    case DownDirection:
        _position -= _worldUp * velocity;
        break;

    default:
        break;
    }
}

void Camera::Rotate(float x, float y)
{
    x *= _rotateSensitivity;
    y *= _rotateSensitivity;

    _yaw += x;
    _pitch = glm::clamp(_pitch + y, -89.0f, 89.0f);

    Update();
}

void Camera::Update()
{
    const auto yawRad = glm::radians(_yaw);
    const auto pitchRad = glm::radians(_pitch);

    _front = glm::normalize(glm::vec3(std::cos(yawRad) * std::cos(pitchRad),
                                      std::sin(pitchRad),
                                      std::sin(yawRad) * std::cos(pitchRad)));
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

}
