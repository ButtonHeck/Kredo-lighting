#pragma once

#include "camera.h"

#include <wx/gdicmn.h>
#include <array>

class wxKeyEvent;
class wxMouseEvent;
class wxEvent;

namespace Kredo
{

class SceneLayer;

class Scene
{
public:
    Scene();
    ~Scene();

    void AddLayer(SceneLayer* layer);

    const Camera& GetCamera() const;

    void ProcessKeyPressed(int keyCode);
    void ProcessKeyReleased(int keyCode);
    void ProcessMouseMove();
    void UpdateOrigin();

    void ProcessEvents();
    void ClearEvents();

    void SetSize(int width, int height);
    void Render();

private:
    Camera _camera;
    std::vector<SceneLayer*> _layers;
    std::array<bool, WXK_WINDOWS_MENU> _keysPressed;
    wxPoint _mousePosition;
};

}
