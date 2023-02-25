#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class wxString;

namespace Kredo
{

class OpenGLShader
{
public:
    OpenGLShader();
    ~OpenGLShader();

    void Load(const wxString& vertexPath, const wxString& fragmentPath);
    void Load(const wxString& vertexPath, const wxString& geometryPath, const wxString& fragmentPath);

    void SetBool(const wxString& name, int value) const;
    void SetInt(const wxString& name, int value) const;
    void SetFloat(const wxString& name, float value) const;
    void SetVec2(const wxString& name, const glm::vec2& vec) const;
    void SetVec2(const wxString& name, float x, float y) const;
    void SetVec3(const wxString& name, const glm::vec3& vec) const;
    void SetVec3(const wxString& name, float x, float y, float z) const;
    void SetMat4(const wxString& name, const glm::mat4& model) const;

    GLuint GetId() const;
    void Use();
    void Cleanup();

private:
    GLuint LoadShader(const wxString& path, GLenum type);
    bool CheckShader(GLuint shader);
    bool CheckProgram();

private:
    GLuint _id;
};

}
