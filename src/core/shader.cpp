#include "shader.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/sstream.h>
#include <wx/wfstream.h>
#include <glm/gtc/type_ptr.hpp>

namespace Kredo
{

Shader::Shader()
    : _id(0)
{
}

Shader::~Shader()
{
    Cleanup();
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
        glDeleteProgram(_id);

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
        glDeleteProgram(_id);

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

void Shader::SetBool(const wxString& name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetInt(const wxString& name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetFloat(const wxString& name, float value) const
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetVec2(const wxString& name, const glm::vec2& vec) const
{
    SetVec2(name, vec.x, vec.y);
}

void Shader::SetVec2(const wxString& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
}

void Shader::SetVec3(const wxString& name, const glm::vec3& vec) const
{
    SetVec3(name, vec.x, vec.y, vec.z);
}

void Shader::SetVec3(const wxString& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void Shader::SetMat4(const wxString& name, const glm::mat4& model) const
{
    const auto loc = glGetUniformLocation(_id, name.ToStdString().c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
}

GLuint Shader::GetId() const
{
    return _id;
}

void Shader::Use()
{
    glUseProgram(_id);
}

void Shader::Cleanup()
{
    glDeleteProgram(_id);
}

GLuint Shader::LoadShader(const wxString& path, GLenum type)
{
    wxFileStream fileStream(path);
    wxStringOutputStream stringStream;
    fileStream.Read(stringStream);

    const auto shaderString = stringStream.GetString().ToStdString();
    const auto shaderSource = shaderString.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    CheckShader(shader);

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
        wxLogDebug(wxString::Format("Shader compilation error: %s", log));
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
        wxLogDebug(wxString::Format("Program link error: %s", log));
        return false;
    }

    return true;
}

}
