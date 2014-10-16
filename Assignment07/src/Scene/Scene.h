#pragma once

class SceneManager;
class SceneNode;

#include "../Utils/Event.h"

#include "../Graphics/ShaderProgram.h"
#include "../EngineObjects/Camera/Camera.h"
#include "SceneNode.h"
#include "../Managers/SceneManager.h"

class Scene
{
	private:
		class SceneNodeIterator
		{
			//define walking through the tree
		};

		/*
		*	fields
		*/
		SceneNode* root;
		ShaderProgram* program;

		void onTick();
		// scene node manager

	public:
		Camera* camera;

		/*
		*	Constructors
		*/
		Scene();
		~Scene();

		/*
		*	Fields
		*/
		Event<double> eventTick;
		Event<Scene*> eventDestroy;

		/*
		*	properties
		*/
		void setShaderProgram(ShaderProgram* program);
		ShaderProgram* getShaderProgram();

		/*
		*	iterators
		*/
		SceneNodeIterator& walkScene();
}; //Scene
