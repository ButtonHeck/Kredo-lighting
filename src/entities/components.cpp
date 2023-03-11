#include "components.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Kredo
{

TagComponent::TagComponent(const wxString& tag)
    : tag(tag)
{
}

TransformComponent::TransformComponent(const glm::vec3& translation)
    : translation(translation)
{
}

glm::mat4 TransformComponent::GetTransform() const
{
    return glm::translate(glm::mat4(1.0f), translation)
           * glm::toMat4(glm::quat(rotation))
           * glm::scale(glm::mat4(1.0f), scale);
}

}
