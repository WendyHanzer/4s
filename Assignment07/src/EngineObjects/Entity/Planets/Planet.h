#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

#include <list>
#include <string>

#include "../Entity.h"
#include "../Aspects/Physics.h"
#include "../Aspects/Renderer.h"

class Planet : public Entity
{
	public:
		std::list<Planet*> moons;

		Physics* physics;
		Renderer* renderer;

		Planet(Planet* origin, std::string mesh, glm::vec3 scale, glm::vec3 position, glm::vec3 rotation = {0, 0, 0}) : Entity();
		//	create scene node
		//	add to origin planet if not NULL, otherwise add to root
		//
		//	create physics aspect
		//	create renderer aspect with mesh
		//
		//	apply scale
		//	apply position
		//	apply rotation
		//
		//	set render to true

		~Planet();
}; //Planet
