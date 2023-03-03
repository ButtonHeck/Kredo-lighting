#pragma once

//temp
#include "camera.h"
#include "pointers.h"
#include "opengl_vertex_array.h"

#include <wx/gdicmn.h>
#include <array>

class wxKeyEvent;
class wxMouseEvent;
class wxEvent;

namespace Kredo
{

class OpenGLShader;

class OpenGLManager
{
public:
    OpenGLManager();
    ~OpenGLManager();

    void ProcessKeyPressed(int keyCode);
    void ProcessKeyReleased(int keyCode);
    void ProcessMouseMove();
    void UpdateOrigin();

    void ProcessEvents();
    void ClearEvents();

    void SetSize(int width, int height);
    void Render();

private:
    // temporary
    unsigned int _width, _height;
    Camera _camera;
    Unique<OpenGLShader> _shader;
    Shared<OpenGLVertexArray> _vertexArray;
    std::array<bool, WXK_WINDOWS_MENU> _keysPressed;
    wxPoint _origin;
    bool _hasMouseMove;
};

}
