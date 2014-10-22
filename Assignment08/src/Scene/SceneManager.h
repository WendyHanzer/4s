#pragma once

#include <unordered_map>
#include <typeindex>

#include "Scene.h"

#include "../Engine/Managers/AspectSourceManager.h"
#include "../Engine/Types/ControllerType.h"

class Scene;

class SceneManager : public AspectSourceManager, public ControllerType { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			std::unordered_map<std::type_index, Scene*> scenes;
/*
*****************************************************************
*
*/	public: /*
*
*
*		Constructors
*		----------------------------------
*/
			SceneManager(Engine& engine);
/*
*		Methods
*		----------------------------------
*/
			template <class T>
			inline void save(T& scene)
			{
				scenes[typeid(scene)] = &scene;
			} //save
			
			template <class T>
			T& load()
			{
				T& scene = dynamic_cast<T&>(*scenes[typeid(T)]);
				scenes.erase(typeid(T));
				return scene;
			} //load
			
			template <class T>
			T* safeLoad()
			{
				auto it = scenes.find(typeid(T));
				
				if (it == scenes.end())
				{
					return NULL;
				} //if
				
				T* scene = it->second();
				
				scenes.erase(typeid(T));
				
				return dynamic_cast<T*>(scene);
			} //safeLoad
/*
*****************************************************************/
}; //SceneManger
