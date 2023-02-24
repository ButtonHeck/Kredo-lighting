#pragma once

#include <wx/string.h>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

namespace Kredo
{

class Shader
{
public:
    Shader();

    void Load(const wxString& vertexPath, const wxString& fragmentPath);
    void Load(const wxString& vertexPath, const wxString& geometryPath, const wxString& fragmentPath);

    GLuint GetId() const;
    void Use();
    void Cleanup();

    void SetMat4(const wxString& name, const glm::mat4& model) const;

private:
    GLuint LoadShader(const wxString& path, GLenum type);
    bool CheckShader(GLuint shader);
    bool CheckProgram();

private:
    GLuint _id;
};

}
