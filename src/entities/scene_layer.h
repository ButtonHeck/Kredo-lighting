#pragma once

namespace Kredo
{

class Scene;

class SceneLayer
{
public:
    explicit SceneLayer(const Scene* const scene);
    virtual ~SceneLayer() = default;

    virtual void Render() const = 0;

protected:
    const Scene* const _scene;
};

}
