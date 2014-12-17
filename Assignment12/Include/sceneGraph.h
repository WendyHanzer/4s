#pragma once

#include <vector>

#include "directionalLight.h"
#include "ambientLight.h"
#include "sceneNode.h"

struct SceneGraph
{
public:
	std::vector<DirectionalLight*> directionalLight;
	std::vector<AmbientLight*> ambientLight;

	SceneNode root;
}; //SceneGraph