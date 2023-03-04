#include <glad/glad.h>

#include "scene.h"
#include "filesystem.h"
#include "test_layer.h"
#include "opengl_shader.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <wx/string.h>

namespace Kredo
{

TestLayer::TestLayer(const Scene* const scene)
    : SceneLayer(scene)
    , _shader(CreateUnique<OpenGLShader>(Filesystem::Path(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, "shaders/basic.vs")),
                                         Filesystem::Path(wxString::Format("%s/%s", KREDO_RESOURCES_DIR, "shaders/basic.fs"))))
{
    _vertexArray = CreateShared<OpenGLVertexArray>();

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,

      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,

      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,

      -0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f
    };

    Shared<OpenGLVertexBuffer> vertexBuffer = CreateShared<OpenGLVertexBuffer>(vertices, sizeof(vertices));
    const auto layout = BufferLayout( {{ShaderDataType::Float3, "i_Pos"}} );

    vertexBuffer->SetLayout(layout);
    _vertexArray->AddVertexBuffer(vertexBuffer);
}

void TestLayer::Render() const
{
    _vertexArray->Bind();

    const glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,   0.0f,  0.0f ),
      glm::vec3(2.0f,   5.0f, -15.0f ),
      glm::vec3(-1.5f, -2.2f, -2.5f ),
      glm::vec3(-3.8f, -2.0f, -12.3f ),
      glm::vec3(2.4f,  -0.4f, -3.5f ),
      glm::vec3(-1.7f,  3.0f, -7.5f ),
      glm::vec3(1.3f,  -2.0f, -2.5f ),
      glm::vec3(1.5f,   2.0f, -2.5f ),
      glm::vec3(1.5f,   0.2f, -1.5f ),
      glm::vec3(-1.3f,  1.0f, -1.5f )
    };

    const auto camera = _scene->GetCamera();

    _shader->Use();
    _shader->SetMat4("u_View", camera.GetViewMatrix());

    const auto projection = glm::perspective(glm::radians(camera.GetFov()), camera.GetAspectRatio(), 0.1f, 100.0f);
    _shader->SetMat4("u_Projection", projection);

    for (auto i = 0; i < 10; i++)
    {
        const auto model = glm::translate(glm::mat4(1.0), cubePositions[i]);
        _shader->SetMat4("u_Model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

}
