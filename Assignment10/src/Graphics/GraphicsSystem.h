#pragma once

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <TGUI/TGUI.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../Engine/System/System.h"
#include "../Entity/Actor/Abstract/Camera.h"
#include "Light/Light.h"
#include "shaderProgram.h"

//#include "../Entity/Actor/Abstract/Camera.h"

class GraphicsSystem : public System
{
	private:
	
		glm::mat4 projection;
		//const glm::mat4 view;	//temp
		Camera& camera;
		glm::mat4 v;
		glm::mat4 vp;
		
		tgui::Gui& layer0;		//menu
		
		tgui::Gui& layer1;		//in front
		//scene
		tgui::Gui& layer2;		//in back
	
	public:
	
		sf::RenderWindow& window;
	
		std::vector<Light*> lights;
		ShaderProgram* program;

		GraphicsSystem(Engine& engine, sf::RenderWindow& window, tgui::Gui& layer0, tgui::Gui& layer1, tgui::Gui& layer2, Camera& camera);
		virtual ~GraphicsSystem();
		
		const glm::mat4 calculateMVP(const glm::mat4& model) { return vp*model; }
		const glm::mat4 calculateMV(const glm::mat4& model) { return v*model; }
		const glm::mat4& calculateVP() { return vp; }
		const glm::mat4& calculateV() { return v; }
		const glm::mat4& calculateP() { return projection; }
		
		inline void updateProjection()
		{	
			projection = glm::perspective(45.0f, float(window.getSize().x)/float(window.getSize().y), .01f, 100.0f);
			glViewport(0, 0, window.getSize().x, window.getSize().y);
		} //updateProjection

	protected:

		virtual void onTick(const long double& delta);

}; //GraphicsSystem
