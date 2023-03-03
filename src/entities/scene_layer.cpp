#include "scene.h"
#include "scene_layer.h"

namespace Kredo
{

SceneLayer::SceneLayer(Shared<Scene> scene)
    : _scene(scene)
{
    _scene->AddLayer(this);
}

}
