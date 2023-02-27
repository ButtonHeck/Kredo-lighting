#pragma once

//temp
#include "camera.h"
#include "opengl_vertex_array.h"

#include <wx/gdicmn.h>
#include <array>
#include <memory>

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

    bool IsInitialized() const;

    void ProcessKeyPressed(int keyCode);
    void ProcessKeyReleased(int keyCode);
    void ProcessMouseMove();
    void UpdateOrigin();

    void ProcessEvents();
    void ClearEvents();

    void SetSize(int width, int height);
    void Render();

private:
    bool _initialized;

    // temporary
    unsigned int _width, _height;
    Camera _camera;
    std::unique_ptr<OpenGLShader> _shader;
    std::shared_ptr<OpenGLVertexArray> _vertexArray;
    std::array<bool, WXK_WINDOWS_MENU> _keysPressed;
    wxPoint _origin;
    bool _hasMouseMove;
};

}
