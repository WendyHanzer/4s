#pragma once

#include "SceneNode.h"

class Scene
{
	public:
	
		SceneNode& root;
		
		Scene() : root(*this) { }
		virtual ~Scene() { }
}; //Scene
