#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class wxString;

namespace Kredo
{

class OpenGLShader
{
public:
    OpenGLShader(const wxString& vertexPath, const wxString& fragmentPath);
    OpenGLShader(const wxString& vertexPath, const wxString& geometryPath, const wxString& fragmentPath);
    ~OpenGLShader();

    void SetBool(const wxString& name, int value) const;
    void SetInt(const wxString& name, int value) const;
    void SetFloat(const wxString& name, float value) const;
    void SetVec2(const wxString& name, const glm::vec2& vec) const;
    void SetVec2(const wxString& name, float x, float y) const;
    void SetVec3(const wxString& name, const glm::vec3& vec) const;
    void SetVec3(const wxString& name, float x, float y, float z) const;
    void SetMat4(const wxString& name, const glm::mat4& model) const;

    uint32_t GetId() const;
    void Use();
    void Cleanup();

private:
    uint32_t LoadShader(const wxString& path, uint32_t type);
    bool CheckShader(uint32_t id);
    bool CheckProgram();

private:
    uint32_t _id;
};

}
