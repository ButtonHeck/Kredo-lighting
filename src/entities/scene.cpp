#include <glad/glad.h>

#include "scene.h"
#include "scene_layer.h"

#include <wx/log.h>
#include <wx/event.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Kredo
{

Scene::Scene()
    : _mousePosition(0, 0)
    , _camera(Camera(glm::vec3(0.0f, 0.0f, 10.0f)))
{
    _keysPressed.fill(false);
    glEnable(GL_DEPTH_TEST);
}

Scene::~Scene()
{
    for (auto layer: _layers)
        delete layer;

    glFinish();
}

void Scene::AddLayer(SceneLayer* layer)
{
    _layers.push_back(layer);
}

const Camera& Scene::GetCamera() const
{
    return _camera;
}

void Scene::ProcessKeyPressed(int keyCode)
{
    _keysPressed[keyCode] = true;
}

void Scene::ProcessKeyReleased(int keyCode)
{
    _keysPressed[keyCode] = false;
}

void Scene::ProcessMouseMove()
{
    int x, y;
    wxGetMousePosition(&x, &y);
    const auto mouseDx = x - _mousePosition.x;
    const auto mouseDy = _mousePosition.y - y;
    _mousePosition = wxPoint(x, y);
    _camera.Rotate(float(mouseDx), float(mouseDy));
}

void Scene::UpdateOrigin()
{
    wxGetMousePosition(&_mousePosition.x, &_mousePosition.y);
}

void Scene::ProcessEvents()
{
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

void Scene::ClearEvents()
{
    _keysPressed.fill(false);
}

void Scene::SetSize(int width, int height)
{
    glViewport(0, 0, width, height);
    _camera.SetAspectRatio(float(width) / float(height));
}

void Scene::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& layer: _layers)
        layer->Render();
}

}
