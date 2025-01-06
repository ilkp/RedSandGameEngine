#define SDL_MAIN_USE_CALLBACKS 1

#include <vector>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "RedSandGame.h"

import RedSandEngine;

using namespace rse;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	if (!SDL_SetAppMetadata("RedSandGame", "1.0", "com.redsandgame"))
		return SDL_AppResult::SDL_APP_FAILURE;

	if (!SDL_Init(SDL_INIT_VIDEO))
		return SDL_AppResult::SDL_APP_FAILURE;

	RedSandGame* game = new RedSandGame();
	if (!game)
		return SDL_AppResult::SDL_APP_FAILURE;
	*appstate = game;

	game->camera = game->entityManager.next();
	game->mesh = game->entityManager.next();

	Camera camera;
	camera.viewport = glm::vec4(0.0f, 0.0f, game->width, game->height);
	camera.projection = glm::perspective(
		glm::radians(45.0f),
		(float)game->width / game->height,
		0.1f,
		100.0f);
	camera.view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	game->cameraStore.set(game->camera, camera);

	Mesh mesh;
	mesh.vertices.push_back(Vertex{ .position = { -10, -10, 0 }, .color = { 1, 0, 0, 1 } });
	mesh.vertices.push_back(Vertex{ .position = { 0, 10, 0 }, .color = { 0, 1, 0, 1 } });
	mesh.vertices.push_back(Vertex{ .position = { 10, -10, 0 }, .color = { 0, 0, 1, 1 } });
	mesh.indices = { 0, 1, 2 };
	game->meshStore.set(game->mesh, mesh);

	Transform transform;
	transform.scale = glm::mat4(1.0f);
	transform.translation = glm::mat4(1.0f);
	transform.translation[3][0] -= 10;
	transform.quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	game->transformStore.set(game->mesh, transform);

	Transform transform2;
	transform2.scale = glm::mat4(1.0f);
	transform2.translation = glm::mat4(1.0f);
	transform2.translation[3][0] += 10;
	transform2.quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	game->transformStore.set(game->mesh2, transform2);

	if (!SDL_CreateWindowAndRenderer("RedSandGame", game->width, game->height, 0, &game->window, &game->renderer))
		return SDL_AppResult::SDL_APP_FAILURE;

	return SDL_AppResult::SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	rse::rseContext().onSdlAppIterate(appstate);
	Key key = rseContext().getKey(SDLK_W);

	RedSandGame* game = static_cast<RedSandGame*>(appstate);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(game->renderer);

	Transform transform = game->transformStore.get(game->mesh);
	transform.quaternion = glm::rotate(transform.quaternion, 0.0001f, glm::vec3(1, 0, 0));
	game->transformStore.set(game->mesh, transform);

	Transform transform2 = game->transformStore.get(game->mesh2);
	transform2.quaternion = glm::rotate(transform2.quaternion, 0.0001f, glm::vec3(0, 1, 0));
	game->transformStore.set(game->mesh2, transform2);

	drawMesh(
		*game->renderer,
		game->cameraStore.get(game->camera),
		game->meshStore.get(game->mesh),
		{ transform, transform2 }
	);

	SDL_RenderPresent(game->renderer);
	return SDL_AppResult::SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	rseContext().onSdlAppEvent(appstate, event);
	switch (event->type)
	{
	case SDL_EventType::SDL_EVENT_QUIT:
	{
		return SDL_APP_SUCCESS;
	}
	}
	return SDL_AppResult::SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	if (appstate != nullptr)
	{
		RedSandGame* game = static_cast<RedSandGame*>(appstate);
		SDL_DestroyRenderer(game->renderer);
		SDL_DestroyWindow(game->window);
		delete(game);
		//SDL_free(game);
	}
}
