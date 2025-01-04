module;
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
export module RedSandEngine:Render;
import :RseTypes;
import :Entity;

export namespace rse
{
	enum class MeshRenderMode
	{
		texture,
		color,
		wireframe
	};

	void drawMesh(
		SDL_Renderer& renderer,
		const Camera& camera,
		const Mesh& mesh,
		const std::vector<Transform>& transforms)
	{
		std::vector<SDL_Vertex> projectedVertices(mesh.vertices.size());
		for (size_t i = 0; i < mesh.vertices.size(); ++i)
		{
			projectedVertices[i].color = mesh.vertices.at(i).color;
			projectedVertices[i].tex_coord = mesh.vertices.at(i).tex_coord;
		}
		for (const Transform& transform : transforms)
		{
			const glm::mat4 modelViewMatrix = camera.view * transform.translation * glm::mat4_cast(transform.quaternion) * transform.scale;
			for (size_t i = 0; i < mesh.vertices.size(); ++i)
			{
				const glm::vec3 projectedPosition = glm::project(
					mesh.vertices[i].position,
					modelViewMatrix,
					camera.projection,
					camera.viewport);
				projectedVertices[i].position = { .x = projectedPosition.x, .y = projectedPosition.y };
			}
			SDL_RenderGeometry(&renderer, nullptr, projectedVertices.data(), projectedVertices.size(), mesh.indices.data(), mesh.indices.size());
		}
	}

	//void drawMesh(SDL_Renderer& renderer, const SdlMesh& mesh, MeshRenderMode mode, SDL_Texture* texture = nullptr)
	//{
	//	switch (mode)
	//	{
	//	case MeshRenderMode::texture:
	//	{
	//		break;
	//	}
	//	case MeshRenderMode::color:
	//	{
	//		SDL_RenderGeometry(&renderer, texture, mesh.vertices.data(), mesh.vertices.size(), mesh.indices.data(), mesh.indices.size());
	//		break;
	//	}
	//	case MeshRenderMode::wireframe:
	//	{
	//		std::vector<SDL_FPoint> points(mesh.indices.size() + mesh.indices.size() / 3);
	//		for (size_t i = 0; i < mesh.indices.size(); i += 3)
	//		{
	//			points[i] = {
	//				.x = mesh.vertices.at(mesh.indices.at(i)).position.x,
	//				.y = mesh.vertices.at(mesh.indices.at(i)).position.y
	//			};
	//			points[i + 1] = {
	//				.x = mesh.vertices.at(mesh.indices.at(i + 1)).position.x,
	//				.y = mesh.vertices.at(mesh.indices.at(i + 1)).position.y
	//			};
	//			points[i + 2] = {
	//				.x = mesh.vertices.at(mesh.indices.at(i + 2)).position.x,
	//				.y = mesh.vertices.at(mesh.indices.at(i + 2)).position.y
	//			};
	//			points[i + 3] = {
	//				.x = points[i].x,
	//				.y = points[i].y
	//			};
	//		}
	//		SDL_RenderLines(&renderer, points.data(), points.size());
	//		break;
	//	}
	//	}
	//}

	void drawLine(
		SDL_Renderer& renderer,
		const glm::vec2& point1,
		const glm::vec2& point2,
		const glm::vec4& color
	)
	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(&renderer, &r, &g, &b, &a);
		SDL_SetRenderDrawColor(&renderer, color.r, color.g, color.b, color.a);
		SDL_RenderLine(&renderer, point1.x, point1.y, point2.x, point2.y);
		SDL_SetRenderDrawColor(&renderer, r, g, b, a);
	}
}