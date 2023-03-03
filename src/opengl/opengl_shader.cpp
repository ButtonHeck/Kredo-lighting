#include "opengl_shader.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/sstream.h>
#include <wx/wfstream.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Kredo
{

OpenGLShader::OpenGLShader()
    : _id(0)
{
}

OpenGLShader::~OpenGLShader()
{
    Cleanup();
}

void OpenGLShader::Load(const wxString& vertexPath, const wxString& fragmentPath)
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

void OpenGLShader::Load(const wxString& vertexPath, const wxString& geometryPath, const wxString& fragmentPath)
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

void OpenGLShader::SetBool(const wxString& name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void OpenGLShader::SetInt(const wxString& name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void OpenGLShader::SetFloat(const wxString& name, float value) const
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void OpenGLShader::SetVec2(const wxString& name, const glm::vec2& vec) const
{
    SetVec2(name, vec.x, vec.y);
}

void OpenGLShader::SetVec2(const wxString& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
}

void OpenGLShader::SetVec3(const wxString& name, const glm::vec3& vec) const
{
    SetVec3(name, vec.x, vec.y, vec.z);
}

void OpenGLShader::SetVec3(const wxString& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void OpenGLShader::SetMat4(const wxString& name, const glm::mat4& model) const
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(model));
}

uint32_t OpenGLShader::GetId() const
{
    return _id;
}

void OpenGLShader::Use()
{
    glUseProgram(_id);
}

void OpenGLShader::Cleanup()
{
    glDeleteProgram(_id);
}

uint32_t OpenGLShader::LoadShader(const wxString& path, uint32_t type)
{
    wxFileStream fileStream(path);
    wxStringOutputStream stringStream;
    fileStream.Read(stringStream);

    const auto shaderString = stringStream.GetString().ToStdString();
    const auto shaderSource = shaderString.c_str();

    const auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    CheckShader(shader);

    return shader;
}

bool OpenGLShader::CheckShader(uint32_t id)
{
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        char log[512];
        glGetShaderInfoLog(id, sizeof(log), nullptr, log);
        wxLogDebug(wxString::Format("OpenGLShader: shader compilation error: %s", log));
        return false;
    }

    return true;
}

bool OpenGLShader::CheckProgram()
{
    int status;
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (status != 1)
    {
        char log[512];
        glGetProgramInfoLog(_id, sizeof(log), nullptr, log);
        wxLogDebug(wxString::Format("OpenGLShader: program link error: %s", log));
        return false;
    }

    return true;
}

}
