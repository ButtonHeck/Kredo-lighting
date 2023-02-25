#pragma once

//temp
#include "camera.h"
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
    unsigned int _vbo, _vao;
    Camera _camera;
    OpenGLShader* const _shader;

    std::array<bool, WXK_WINDOWS_MENU> _keysPressed;
    wxPoint _origin;
    bool _hasMouseMove;
};

}
