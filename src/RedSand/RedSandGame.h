#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

import RedSandEngine;

using namespace rse;

struct RedSandGame
{
	static const int width = 600;
	static const int height = 400;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Entity camera;
	Entity mesh;
	Entity mesh2;
	EntityManager entityManager;
	CompactStore<Transform> transformStore{};
	CompactStore<Camera> cameraStore{};
	CompactStore<glm::mat4> mvpStore{};
	CompactStore<Mesh> meshStore{};
};

