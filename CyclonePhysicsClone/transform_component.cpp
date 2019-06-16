
#include "transform_component.h"

TransformComponent::TransformComponent()
{
	position = glm::vec3(0.0, 0.0, 0.0);
	velocity = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	rotation = glm::mat4(1.0);
}






