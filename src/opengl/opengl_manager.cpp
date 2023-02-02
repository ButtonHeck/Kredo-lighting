#include "opengl/opengl_manager.h"

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
}

}
