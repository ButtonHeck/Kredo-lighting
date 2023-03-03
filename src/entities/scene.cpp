#include <glad/glad.h>

#include "scene.h"
#include "filesystem.h"
#include "scene_layer.h"

#include <wx/log.h>
#include <wx/event.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Kredo
{

Scene::Scene()
    : _origin(0, 0)
    , _camera(Camera(glm::vec3(0.0f, 0.0f, 10.0f)))
    , _hasMouseMove(false)
{
    _keysPressed.fill(false);
    glEnable(GL_DEPTH_TEST);
}

Scene::~Scene()
{
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
    _hasMouseMove = true;
}

void Scene::UpdateOrigin()
{
    wxGetMousePosition(&_origin.x, &_origin.y);
}

void Scene::ProcessEvents()
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

void Scene::ClearEvents()
{
    _hasMouseMove = false;
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
