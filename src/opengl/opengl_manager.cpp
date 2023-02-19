#include "opengl_manager.h"

#include <wx/log.h>
#include <wx/event.h>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

//temp
const char* shaderVs = R"glsl(
#version 450

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)glsl";

const char* shaderFs = R"glsl(
#version 450

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5, 1.0, 1.0, 1.0);
}
)glsl";

namespace Kredo
{

OpenGLManager::OpenGLManager()
    : _width(0)
    , _height(0)
    , _mouseX(0)
    , _mouseY(0)
    , _hasMouseMove(0)
{
    _initialized = gladLoadGL() != 0;
    if (_initialized)
    {
        _shader.Initialize(shaderVs, shaderFs);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        const GLfloat vertices[] = {
          -0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,

          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,

           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,

          -0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f,  0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f
        };

        glCreateVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glCreateBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
    }
}

OpenGLManager::~OpenGLManager()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vao);
    glFinish();
}

bool OpenGLManager::IsInitialized() const
{
    return _initialized;
}

void OpenGLManager::ProcessKeyPressed(int keyCode)
{
    _keysPressed[keyCode] = true;
}

void OpenGLManager::ProcessKeyReleased(int keyCode)
{
    _keysPressed[keyCode] = false;
}

void OpenGLManager::ProcessMouseMove()
{
    _hasMouseMove = true;
}

void OpenGLManager::ProcessEvents()
{
    if (_hasMouseMove)
    {
        int mouseDx = 0;
        int mouseDy = 0;
        int x, y;
        wxGetMousePosition(&x, &y);
        wxLogDebug(wxString::Format("process mouse event x %d, y %d", x, y));
        if (_firstMouseMove)
        {
            _mouseX = x;
            _mouseY = y;
            _firstMouseMove = false;
        }
        else
        {
            mouseDx = x - _mouseX;
            mouseDy = _mouseY - y;
            _mouseX = x;
            _mouseY = y;
        }

        _camera.Rotate(float(mouseDx), float(mouseDy));
        _hasMouseMove = false;
    }

    if (_keysPressed['W'])
        _camera.Move(Camera::ForwardDirection, 0.1f);
    if (_keysPressed['S'])
        _camera.Move(Camera::BackwardDirection, 0.1f);
    if (_keysPressed['A'])
        _camera.Move(Camera::LeftDirection, 0.1f);
    if (_keysPressed['D'])
        _camera.Move(Camera::RightDirection, 0.1f);
    if (_keysPressed[WXK_SPACE])
        _camera.Move(Camera::UpDirection, 0.1f);
    if (_keysPressed[WXK_SHIFT])
        _camera.Move(Camera::DownDirection, 0.1f);
}

void OpenGLManager::SetSize(int width, int height)
{
    if (_initialized)
    {
        _width = width;
        _height = height;
        glViewport(0, 0, _width, _height);
    }
}

void OpenGLManager::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,   0.0f,  0.0f ),
      glm::vec3(2.0f,   5.0f, -15.0f ),
      glm::vec3(-1.5f, -2.2f, -2.5f ),
      glm::vec3(-3.8f, -2.0f, -12.3f ),
      glm::vec3(2.4f,  -0.4f, -3.5f ),
      glm::vec3(-1.7f,  3.0f, -7.5f ),
      glm::vec3(1.3f,  -2.0f, -2.5f ),
      glm::vec3(1.5f,   2.0f, -2.5f ),
      glm::vec3(1.5f,   0.2f, -1.5f ),
      glm::vec3(-1.3f,  1.0f, -1.5f )
    };

    _shader.Use();
    _shader.SetMat4("view", _camera.GetViewMatrix());

    glm::mat4 projection = glm::perspective(glm::radians(_camera.GetFov()), float(_width) / float(_height), 0.1f, 100.0f);
    _shader.SetMat4("projection", projection);

    for (auto i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, cubePositions[i]);
        _shader.SetMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

}
