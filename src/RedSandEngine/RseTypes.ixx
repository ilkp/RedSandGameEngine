module;
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL3/SDL.h>
export module RedSandEngine:RseTypes;

import <vector>;

export namespace rse
{
	using OnSdlEvent = std::function<void(SDL_Event* event)>;
	using OnSdlIterate = std::function<void()>;

	struct Vertex
	{
		glm::vec3 position;
		SDL_FColor color;
		SDL_FPoint tex_coord;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};

	template<class T>
	struct Bounds
	{
		T min;
		T max;
	};
	using Bounds2d = Bounds<glm::vec2>;
	using Bounds3d = Bounds<glm::vec3>;

	struct Transform
	{
		glm::mat4 position = glm::mat4(1.0f);
		glm::mat4 scale = glm::mat4(1.0f);
		glm::quat quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);;

		glm::vec3 getPosition() const
		{
			return glm::vec3(position[3][0], position[3][1], position[3][2]);
		}

		glm::vec3 getScale() const
		{
			return glm::vec3(scale[0][0], scale[1][1], scale[2][2]);
		}

		void setPosition(float x, float y, float z)
		{
			position = glm::mat4(1.0f);
			position[3][0] = x;
			position[3][1] = y;
			position[3][2] = z;
		}

		void setPosition(const glm::vec3& position)
		{
			this->position = glm::mat4(1.0f);
			this->position[3][0] = position.x;
			this->position[3][1] = position.y;
			this->position[3][2] = position.z;
		}

		void setScale(float x, float y, float z)
		{
			scale = glm::mat4(1.0f);
			scale[0][0] = x;
			scale[1][1] = y;
			scale[2][2] = z;
		}

		void setScale(const glm::vec3& scale)
		{
			this->scale = glm::mat4(1.0f);
			this->scale[0][0] = scale.x;
			this->scale[1][1] = scale.y;
			this->scale[2][2] = scale.z;
		}

		void setRotation(const glm::quat& rotation)
		{
			this->quaternion = rotation;
		}

		void setRotation(const glm::mat4& rotation)
		{
			this->quaternion = glm::quat_cast(rotation);
		}

		void translate(float x, float y, float z)
		{
			position[3][0] += x;
			position[3][1] += y;
			position[3][2] += z;
		}

		void translate(const glm::vec3& translation)
		{
			position[3][0] += translation.x;
			position[3][1] += translation.y;
			position[3][2] += translation.z;
		}

		void rotate(float angle, const glm::vec3& axis)
		{
			quaternion = glm::rotate(quaternion, angle, axis);
		}
	};

	struct Camera
	{
		glm::vec4 viewport;
		glm::mat4 projection;
		glm::mat4 view;

		void applyTransform(const Transform& transform)
		{
			const glm::mat4 model = transform.position * glm::mat4_cast(transform.quaternion) * transform.scale;
			const glm::mat4 tmodel = glm::transpose(model);
			const glm::vec3 forward = glm::normalize(glm::vec3(tmodel[2])) * glm::vec3(1, 1, -1);
			view = glm::lookAt(
				transform.getPosition(),
				forward,
				glm::vec3(0.0f, -1.0f, 0.0f));
		}
	};
}