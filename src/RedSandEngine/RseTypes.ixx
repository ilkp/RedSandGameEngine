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

		glm::vec3 getPosition() const;
		glm::vec3 getScale() const;
		glm::vec3 forward() const;
		void setPosition(float x, float y, float z);
		void setPosition(const glm::vec3& position);
		void setScale(float x, float y, float z);
		void setScale(const glm::vec3& scale);
		void setRotation(const glm::quat& rotation);
		void setRotation(const glm::mat4& rotation);
		void translate(float x, float y, float z);
		void translate(const glm::vec3& translation);
		void rotate(float angle_rad, const glm::vec3& axis);
	};

	struct Camera
	{
		glm::vec4 viewport;
		glm::mat4 projection;
		glm::mat4 view;

		void applyTransform(const Transform& transform);
	};

	class Collider
	{
	public:
		enum class Type
		{
			box,
			sphere
		};

		Collider() = delete;
		Collider(Type type) : _type(type) {}

	protected:
		Type _type;
	};
}