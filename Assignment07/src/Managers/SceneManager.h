#pragma once

#include <string>
#include <map>
#include <vector>

#include "Engine.h"
#include "../Scene/Scene.h"

class SceneManager { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			std::vector<Scene*> scenes;

/*		Constructors
*		----------------------------
*/
			SceneManager(Engine* engine);
			~SceneManager();
/*
*****************************************************************
*
*/	public: /*
*
*
*		Fields
*		----------------------------------
*/
			Event<void> eventStart;
			Event<void> eventStop;

			int addScene(Scene* scene);

			void removeScene(Scene* scene);
			void removeScene(const int& sceneId);
/*
*		Properties
*		----------------------------------
*/


/*
*****************************************************************/

}; //SceneManger
