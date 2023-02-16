#pragma once

#include <wx/string.h>
#include <glm/mat4x4.hpp>

namespace Kredo
{

class Shader
{
public:
    Shader();

    void Initialize(const char* vertex, const char* fragment);
    unsigned int Id() const;
    void Use();
    void SetMat4(const wxString& name, glm::mat4 model ) const;
    void Cleanup();

private:
    unsigned int _id;
};

}
