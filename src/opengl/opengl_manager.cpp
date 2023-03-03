#include <glad/glad.h>

#include "filesystem.h"
#include "opengl_shader.h"
#include "opengl_buffer.h"
#include "opengl_manager.h"

#include <wx/log.h>
#include <wx/event.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Kredo
{

OpenGLManager::OpenGLManager()
    : _width(0)
    , _height(0)
    , _origin(0, 0)
    , _camera(Camera(glm::vec3(0.0f, 0.0f, 10.0f)))
    , _hasMouseMove(false)
    , _shader(CreateUnique<OpenGLShader>(Filesystem::Path(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, "shaders/basic.vs")),
                                         Filesystem::Path(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, "shaders/basic.fs"))))
{
    _keysPressed.fill(false);
    _vertexArray = CreateShared<OpenGLVertexArray>();

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
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

    Shared<OpenGLVertexBuffer> vertexBuffer = CreateShared<OpenGLVertexBuffer>(vertices, sizeof(vertices));
    const auto layout = BufferLayout( {{ShaderDataType::Float3, "i_Pos"}} );

    vertexBuffer->SetLayout(layout);
    _vertexArray->AddVertexBuffer(vertexBuffer);
}

OpenGLManager::~OpenGLManager()
{
    glFinish();
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

void OpenGLManager::UpdateOrigin()
{
    wxGetMousePosition(&_origin.x, &_origin.y);
}

void OpenGLManager::ProcessEvents()
{
    if (_hasMouseMove)
    {
        int x, y;
        wxGetMousePosition(&x, &y);
        const auto mouseDx = x - _origin.x;
        const auto mouseDy = _origin.y - y;
        _origin = wxPoint(x, y);
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

void OpenGLManager::ClearEvents()
{
    _hasMouseMove = false;
    _keysPressed.fill(false);
}

void OpenGLManager::SetSize(int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height);
}

void OpenGLManager::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _vertexArray->Bind();

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

    _shader->Use();
    _shader->SetMat4("u_View", _camera.GetViewMatrix());

    const auto projection = glm::perspective(glm::radians(_camera.GetFov()), float(_width) / float(_height), 0.1f, 100.0f);
    _shader->SetMat4("u_Projection", projection);

    for (auto i = 0; i < 10; i++)
    {
        const auto model = glm::translate(glm::mat4(1.0), cubePositions[i]);
        _shader->SetMat4("u_Model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

}
