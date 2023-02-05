#include "opengl_manager.h"

#include <wx/log.h>
#include <glad/glad.h>

namespace Kredo
{

OpenGLManager::OpenGLManager()
{
    _initialized = gladLoadGL() != 0;
}

OpenGLManager::~OpenGLManager()
{
    glFinish();
}

bool OpenGLManager::IsInitialized() const
{
    return _initialized;
}

void OpenGLManager::SetSize(int width, int height)
{
    if (_initialized)
        glViewport(0, 0, width, height);
}

void OpenGLManager::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float points[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    GLuint _vbo, _vao;

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}
