#pragma once

#include "uuid.h"

#include <wx/string.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Kredo
{

struct IDComponent
{
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;

    UUID id;
};

struct TagComponent
{
    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const wxString& tag);

    wxString tag;
};

struct TransformComponent
{
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation);

    glm::mat4 GetTransform() const;

    glm::vec3 translation ={0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};

}
