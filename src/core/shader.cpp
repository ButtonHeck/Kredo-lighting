#include "shader.h"

#include <wx/log.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Kredo
{

Shader::Shader()
    : _id(0)
{
}

void Shader::Initialize(const char* vertex, const char* fragment)
{
    int status;
    char logBuffer[512];

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        glGetShaderInfoLog(vs, sizeof(logBuffer), nullptr, logBuffer);
        wxLogError("Shader vertex failed to compile");
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        glGetShaderInfoLog(fs, sizeof(logBuffer), nullptr, logBuffer);
        wxLogError("Shader fragment failed to compile");
    }

    _id = glCreateProgram();
    glAttachShader(_id, vs);
    glAttachShader(_id, fs);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (status != 1)
    {
        glGetProgramInfoLog(_id, sizeof(logBuffer), nullptr, logBuffer);
        wxLogError("Shader program link error");
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

unsigned int Shader::Id() const
{
    return _id;
}

void Shader::Use()
{
    glUseProgram(_id);
}

void Shader::SetMat4(const wxString& name, glm::mat4 model) const
{
    const auto loc = glGetUniformLocation(_id, name.ToStdString().c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::Cleanup()
{
    glDeleteProgram(_id);
}

}
