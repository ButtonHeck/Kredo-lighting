#pragma once

//temp
#include "camera.h"
#include "shader.h"

class wxKeyEvent;
class wxMouseEvent;
class wxEvent;

namespace Kredo
{

class OpenGLManager
{
public:
    OpenGLManager();
    ~OpenGLManager();

    bool IsInitialized() const;

    void ProcessKeyPressed(int keyCode);
    void ProcessKeyReleased(int keyCode);
    void ProcessMouseMove();

    void ProcessEvents();
    void SetSize(int width, int height);
    void Render();

private:
    bool _initialized;

    // temporary
    unsigned int _width, _height;
    unsigned int _vbo, _vao;
    Camera _camera;
    Shader _shader;

    bool _keysPressed[WXK_WINDOWS_MENU] = {false};
    int _mouseX;
    int _mouseY;
    bool _hasMouseMove;
    bool _firstMouseMove = true;
    bool _hasMouseEvent = false;
};

}
