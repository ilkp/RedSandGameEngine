module;
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL3/SDL.h>
export module RedSandEngine:RseTypes;

export namespace rse
{
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
		glm::mat4 translation;
		glm::mat4 scale;
		glm::quat quaternion;
	};

	struct Camera
	{
		glm::vec4 viewport;
		glm::mat4 projection;
		glm::mat4 view;
	};
}