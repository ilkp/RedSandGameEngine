module;
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
module RedSandEngine:RseTypes;

namespace rse
{
	glm::vec3 Transform::getPosition() const
	{
		return glm::vec3(position[3][0], position[3][1], position[3][2]);
	}

	glm::vec3 Transform::getScale() const
	{
		return glm::vec3(scale[0][0], scale[1][1], scale[2][2]);
	}

	glm::vec3 Transform::forward() const
	{
		return quaternion * glm::vec3(0, 0, 1);
	}

	void Transform::setPosition(float x, float y, float z)
	{
		position = glm::mat4(1.0f);
		position[3][0] = x;
		position[3][1] = y;
		position[3][2] = z;
	}

	void Transform::setPosition(const glm::vec3& position)
	{
		this->position = glm::mat4(1.0f);
		this->position[3][0] = position.x;
		this->position[3][1] = position.y;
		this->position[3][2] = position.z;
	}

	void Transform::setScale(float x, float y, float z)
	{
		scale = glm::mat4(1.0f);
		scale[0][0] = x;
		scale[1][1] = y;
		scale[2][2] = z;
	}

	void Transform::setScale(const glm::vec3& scale)
	{
		this->scale = glm::mat4(1.0f);
		this->scale[0][0] = scale.x;
		this->scale[1][1] = scale.y;
		this->scale[2][2] = scale.z;
	}

	void Transform::setRotation(const glm::quat& rotation)
	{
		quaternion = rotation;
	}

	void Transform::setRotation(const glm::mat4& rotation)
	{
		quaternion = glm::quat_cast(rotation);
	}

	void Transform::translate(float x, float y, float z)
	{
		position[3][0] += x;
		position[3][1] += y;
		position[3][2] += z;
	}

	void Transform::translate(const glm::vec3& translation)
	{
		position[3][0] += translation.x;
		position[3][1] += translation.y;
		position[3][2] += translation.z;
	}

	void Transform::rotate(float angle_rad, const glm::vec3& axis)
	{
		quaternion = glm::rotate(quaternion, angle_rad, axis);
	}

	void Camera::applyTransform(const Transform& transform)
	{
		const glm::mat4 model = transform.position * glm::mat4_cast(transform.quaternion) * transform.scale;
		const glm::mat4 tmodel = glm::transpose(model);
		const glm::vec3 forward = glm::normalize(glm::vec3(tmodel[2])) * glm::vec3(1, 1, -1);
		const glm::vec3 position = transform.getPosition();
		view = glm::lookAt(
			position,
			position + forward,
			glm::vec3(0.0f, -1.0f, 0.0f));
	}
}