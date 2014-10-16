#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera/Camera.h"
#include "../Planet/Entity.h"
#include "Physics.h"
#include "AspectType.h"
#include "../../Scene/Scenes/SceneNode.h"
#include "../../Graphics/Mesh.h"
#include "../../Managers/MeshManager.h"
#include "../../Managers/GraphicsManager.h"

class Render { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
		Entity* entity;
		Mesh* mesh;

		static AspectType* aspectType;

		void onDestroy(Entity* entity);

		void onRender();
		//	registered to GraphicsManager eventRender
		//	renders if visible and mesh not NULL

		~Render();

/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------
*/
		Render(Entity* entity) : AspectType(aspectType), entity(entity);


/*
*		Methods
*		----------------------------------
*/
		bool intersectsPoint(glm::vec3 point);

		glm::vec3 calculateIntersection(Entity* entity);

		bool visible;
		//  starts as false


		 void loadMesh(std::string meshName);
		 //	load mesh or NULL

/*
*		Properties
*		----------------------------------
*/

/*
*****************************************************************/
}; //Render