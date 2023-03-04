#pragma once

#include "camera.h"
#include "scene_layer.h"
#include "opengl_vertex_array.h"

namespace Kredo
{

class OpenGLShader;

class TestLayer : public SceneLayer
{
public:
    explicit TestLayer(const Scene* const scene);
    ~TestLayer() = default;

    void Render() const override;

private:
    Camera _camera;
    Unique<OpenGLShader> _shader;
    Shared<OpenGLVertexArray> _vertexArray;
};

}
