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
    : _name("")
    , _mousePosition(0, 0)
    , _camera()
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

void Scene::SetName(const wxString& name)
{
    _name = name;
}

const wxString& Scene::GetName() const
{
    return _name;
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

void Scene::ProcessMouseMove(const wxPoint& mousePosition, bool warped)
{
    if (!warped)
    {
        const auto dx = mousePosition.x - _mousePosition.x;
        const auto dy = _mousePosition.y - mousePosition.y;
        _camera.Rotate(float(dx), float(dy));
    }

    _mousePosition = mousePosition;
}

void Scene::ProcessKeyboard()
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

void Scene::ClearKeyboard()
{
    _keysPressed.fill(false);
}

void Scene::SetSize(int width, int height)
{
    glViewport(0, 0, width, height);
    _camera.SetViewport(width, height);
}

void Scene::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& layer: _layers)
        layer->Render();
}

}
