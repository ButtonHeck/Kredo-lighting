#pragma once

#include "pointers.h"

namespace Kredo
{

class Scene;

class SceneLayer
{
public:
    explicit SceneLayer(Shared<Scene> scene);
    virtual ~SceneLayer() = default;

    virtual void Render() const = 0;

protected:
    Shared<Scene> _scene;
};

}
