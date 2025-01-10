module;
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
export module RedSandEngine:Render;

import :RseTypes;
import :Entity;

import <vector>;

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
		std::vector<glm::vec3> projectedPositions(mesh.vertices.size());
		std::vector<SDL_Vertex> projectedSdlVertices(mesh.vertices.size());
		for (const Transform& transform : transforms)
		{
			const glm::mat4 modelViewMatrix = camera.view * transform.position * glm::mat4_cast(transform.quaternion) * transform.scale;
			for (size_t i = 0; i < mesh.vertices.size(); ++i)
			{
				projectedPositions[i] = glm::project(
					mesh.vertices[i].position,
					modelViewMatrix,
					camera.projection,
					camera.viewport);
				projectedSdlVertices[i].position = { .x = projectedPositions.at(i).x, .y = projectedPositions.at(i).y };
				projectedSdlVertices[i].color = mesh.vertices.at(i).color;
				projectedSdlVertices[i].tex_coord = mesh.vertices.at(i).tex_coord;
			}

			std::vector<int> clippedIndices;
			for (size_t i = 0; i < mesh.indices.size(); i = i + 3)
			{
				const glm::vec3& p1 = projectedPositions.at(mesh.indices.at(i));
				const glm::vec3& p2 = projectedPositions.at(mesh.indices.at(i + 1));
				const glm::vec3& p3 = projectedPositions.at(mesh.indices.at(i + 2));

				if (p1.z < 1.0f || p2.z < 1.0f || p3.z < 1.0f)
					continue;

				const glm::vec3 normal = glm::cross(
					p3 - p1,
					p2 - p1
				);

				const bool facingCamera = glm::dot(p1, normal) < 0;
				if (!facingCamera)
					continue;

				clippedIndices.push_back(mesh.indices.at(i));
				clippedIndices.push_back(mesh.indices.at(i + 1));
				clippedIndices.push_back(mesh.indices.at(i + 2));
			}

			SDL_RenderGeometry(
				&renderer,
				nullptr,
				projectedSdlVertices.data(), projectedSdlVertices.size(),
				clippedIndices.data(), clippedIndices.size());
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