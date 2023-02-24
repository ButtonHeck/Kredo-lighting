#include "shader.h"

#include <wx/log.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace Kredo
{

Shader::Shader()
    : _id(0)
{
}

void Shader::Load(const wxString& vertexPath, const wxString& fragmentPath)
{
    const auto vertexShader = LoadShader(vertexPath, GL_VERTEX_SHADER);
    const auto fragmentShader = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    if (!CheckProgram())
        wxLogError("Shader program link error");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Load(const wxString& vertexPath, const wxString& geometryPath, const wxString& fragmentPath)
{
    const auto vertexShader = LoadShader(vertexPath, GL_VERTEX_SHADER);
    const auto geometryShader = LoadShader(geometryPath, GL_GEOMETRY_SHADER);
    const auto fragmentShader = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, geometryShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    if (!CheckProgram())
        wxLogError("Shader program link error");

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::GetId() const
{
    return _id;
}

void Shader::Use()
{
    glUseProgram(_id);
}

void Shader::SetMat4(const wxString& name, const glm::mat4 &model) const
{
    const auto loc = glGetUniformLocation(_id, name.ToStdString().c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::Cleanup()
{
    glDeleteProgram(_id);
}

GLuint Shader::LoadShader(const wxString& path, GLenum type)
{
    std::fstream shaderFile(path);
    std::stringstream shaderStringBuffer;
    shaderStringBuffer << shaderFile.rdbuf();
    const auto shaderString = shaderStringBuffer.str();
    const auto shaderSource = shaderString.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    if (!CheckShader(shader))
        wxLogError(wxString::Format("Error while loading shader %s", path));

    return shader;
}

bool Shader::CheckShader(GLuint shader)
{
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        wxLogDebug(log);
        return false;
    }

    return true;
}

bool Shader::CheckProgram()
{
    int status;
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (status != 1)
    {
        char log[512];
        glGetProgramInfoLog(_id, sizeof(log), nullptr, log);
        wxLogDebug(log);
        return false;
    }

    return true;
}

}
