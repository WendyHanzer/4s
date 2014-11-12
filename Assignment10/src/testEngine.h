#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

#include <iostream>

#include "Engine/Engine.h"
#include "Engine/Entity/Entity.h"
#include "Physics/PhysicsSystem.h"
#include "Input/InputSystem.h"
#include "AI/AISystem.h"

#include "Entity/Actor/Component/ActorPhysicsComponent.h"
#include "Entity/Actor/Component/ActorGraphicsComponent.h"
#include "Entity/Actor/Actor.h"
#include "Entity/Actor/ActorManager.h"
#include "Entity/Actor/Abstract/Camera.h"
#include "Entity/Actor/Component/AI/Command/ActorCommandFollow.h"
#include "Entity/Actor/Component/AI/Command/ActorCommandMove.h"
#include "Entity/Actor/Component/AI/Command/ActorCommandStop.h"

#include "Graphics/GraphicsSystem.h"
#include "Graphics/mesh.h"
#include "Graphics/shader.h"
#include "Graphics/shaderProgram.h"

#include "Player/Player.h"
#include "Player/Human.h"
#include "Player/Computer.h"
#include "Player/AI/BalancedAI.h"

#include "Player/Control/MouseControl.h"
#include "Player/Control/CameraControl.h"

#include "Graphics/Light/DirectionalLight.h"
#include "Graphics/Light/PointLight.h"
#include "Graphics/Light/SpotLight.h"

using namespace std;

class CameraInputControl : public Component
{
	private:
	
		Event<const long double&>::id actionTickEngine;
		
		const sf::Window& window;
		
		sf::Vector2<int> prevMousePosition;

	protected:

		virtual void onChangeController()
		{
		} //onChangeController

		virtual void onStart()
		{
			actionTickEngine->enable = true;
		
			Component::onStart();
		} //onStart

		virtual void onStop()
		{
			actionTickEngine->enable = false;
			
			Component::onStop();
		} //onStop

		virtual void onTick(const long double& delta)
		{
			/*
			*	mouse control
			*/
			switch (((InputSystem&)(System&)mapping).inputResponse.type)
			{
				case sf::Event::MouseWheelMoved:
					break;
					((Camera&)getController()).zoom(-((InputSystem&)(System&)mapping).inputResponse.mouseWheel.delta*delta*45);
					//std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;
					//std::cout << "mouse x: " << event.mouseWheel.x << std::endl;
					//std::cout << "mouse y: " << event.mouseWheel.y << std::endl;

					break;
				case sf::Event::MouseMoved:
					//mouseActor.setTransformation(glm::translate(mouseActor.getTransformation(), glm::vec3(
						//(((InputSystem&)(System&)mapping).inputResponse.mouseMove.x - prevMousePosition.x)*delta*.19, 0,
						//(((InputSystem&)(System&)mapping).inputResponse.mouseMove.y - prevMousePosition.y)*delta*.19)));
					//break;
					((Camera&)getController()).rotate(glm::vec3(
											(((InputSystem&)(System&)mapping).inputResponse.mouseMove.y - prevMousePosition.y)*delta*.03,
											(((InputSystem&)(System&)mapping).inputResponse.mouseMove.x - prevMousePosition.x)*delta*.03,
											0));
				
					break;

				default:
					break;
			} //switch
			
			Component::onTick(delta);
		} //onTick
		
		virtual void onTickEngine(const long double& delta)
		{
			/*
			*	keyboard control
			*/
			static const float SENSITIVITY = 10;
			static const float SENSITIVITY_2 = 2.5;

			glm::vec3 move(0, 0, 0);
			glm::vec3 euler(0, 0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				move.x -= SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				move.x += SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				move.z -= SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				move.z += SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				euler.y -= SENSITIVITY_2*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				euler.y += SENSITIVITY_2*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				euler.x -= SENSITIVITY_2*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				euler.x += SENSITIVITY_2*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
			{
				move.y -= SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
			{
				move.y += SENSITIVITY*delta;
			} //if
			
			((Camera&)getController()).translate(move);
			
			sf::Mouse::setPosition(prevMousePosition, window);
			
			((Camera&)getController()).rotate(euler);
		} //onTickEngine

	public:
	
		Actor& mouseActor;

		CameraInputControl(Actor& controller, const string& name, Engine& engine, const sf::Window& window) :
			Component(controller, (System&)controller.getManager().context[typeid(InputSystem)], typeid(InputSystem), name),
			actionTickEngine(engine.event.tick.reg(&CameraInputControl::onTickEngine, this)),
			window(window),
			prevMousePosition(window.getSize().x/2, window.getSize().y/2),
			mouseActor(*new Actor((ActorManager&)controller.getManager(), "mouse"))
		{
		} //CameraInputControl

		virtual ~CameraInputControl()
		{
			actionTickEngine->unreg();
			
			destroy();
		} //~CameraInputControl
}; //CameraInputControl

class TestEngine : public Engine
{
	sf::RenderWindow window;

	tgui::Gui gui0;
	tgui::Gui gui1;
	tgui::Gui gui2;

	ActorManager& actorManager;
	
	Camera& camera;

	GraphicsSystem& graphics;
	InputSystem& input;

	Shader vertexShader;
	Shader fragmentShader;
	ShaderProgram program;
	
	DirectionalLight* ambientLight;
	PointLight* pointLight;
	std::vector<SpotLight*> spotLights;
	DirectionalLight* directionalLight;
	
	int selected = 0;

	Mesh* mesh;
	Actor* actor;
	
	bool active(Light* light)
	{
		for (Light* l : graphics.lights)
		{
			if (l == light)
			{
				return true;
			} //if
		} //for
		
		return false;
	} //active
	
	void toggle(Light* light)
	{
		for (unsigned int i = 0; i < graphics.lights.size(); ++i)
		{
			if (graphics.lights[i] == light)
			{
				graphics.lights.erase(graphics.lights.begin() + i);
			
				return;
			} //if
		} //for
		
		graphics.lights.push_back(light);
	} //toggle

	void updateProjection(const long double& delta)
	{
		switch (input.inputResponse.type)
		{
			case sf::Event::Closed:
				window.close();

				break;
			case sf::Event::Resized:
				graphics.updateProjection();

				break;
			case sf::Event::KeyPressed:
				switch(input.inputResponse.key.code)
				{
					case sf::Keyboard::Tab:
						selected = (selected + 1)%spotLights.size();
						break;
					case sf::Keyboard::Num1:
						toggle(ambientLight);
						break;
					case sf::Keyboard::Num2:
						toggle(directionalLight);
						break;
					case sf::Keyboard::Num3:
						toggle(pointLight);
						break;
					case sf::Keyboard::Space:
						if (spotLights.size() > 0)
						{
							toggle(spotLights[selected]);
						} //if
						break;
					case sf::Keyboard::Return:
						if (spotLights.size() < 8)
						{
							SpotLight* spotLight = new SpotLight();
							spotLight->direction = glm::normalize(glm::vec3(-1, 1, 0));
							spotLight->position = glm::vec3(3, -4, 0);
							spotLight->color = glm::vec3(0, .8, .8);
							spotLight->diffuseIntensity = 16;
							spotLight->ambientIntensity = 0;
							spotLight->attenuation.constant = 1;
							spotLight->attenuation.linear = .4;
							spotLight->attenuation.exp = .2;
							spotLight->cutoff = glm::pi<float>()/32;
							spotLights.push_back(spotLight);
							graphics.lights.push_back(spotLight);
						} //if
						break;
					default:
						break;
				} //switch

				break;
				
			default:
				break;
		} //switch
	} //updateProjection

	void moveSelected(const long double& delta)
	{
		glm::vec2 translation(0, 0);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			translation.x += .1*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			translation.x -= .1*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			translation.y -= .1*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			translation.y += .1*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			stop();
		} //if
		
		if (glm::length(translation) == 0)
		{
			return;
		} //if
		
		//float y = translation.y;
		//translation.y = translation.x;
		//translation.x = y;
		
		//rotate spotlight
		glm::vec3& direction = spotLights[selected]->direction;
		direction.z += translation.y;
		direction.y += translation.x;
	} //select

	static void quitCallback(const tgui::Callback& callback)
	{
		//stop();
	}

	void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2)
	{
		tgui::Picture::Ptr picture(gui2);
		picture->load("images/xubuntu_bg_aluminium.jpg");
		picture->setCallbackId(3);

		// Create the quit button
		/*
		tgui::Button::Ptr quit(gui);
		quit->load("widgets/Black.conf");
		quit->setSize(128, 30);
		quit->setPosition(window.getSize().x - 128, 0);
		quit->setText("Quit");
		quit->setCallbackId(2);
		quit->bindCallbackEx(&TestEngine::quitCallback, tgui::Button::LeftMouseClicked);
		*/
	} //loadWidgets

	public:

		TestEngine(int argc, char** argv) :
			Engine(),
			window(sf::VideoMode(800, 600), "Test Engine", sf::Style::Default, sf::ContextSettings(32, 0, 0, 3, 3)),
			gui0(window),
			gui1(window),
			gui2(window),
			actorManager(*new ActorManager(*this)),
			camera(*new Camera(actorManager)),
			graphics(*new GraphicsSystem(*this, window, gui0, gui1, gui2, camera)),
			input(*new InputSystem(*this, window, gui0, gui1, gui2)),
			vertexShader(GL_VERTEX_SHADER),
			fragmentShader(GL_FRAGMENT_SHADER)
		{
			window.setMouseCursorVisible(false);
		
			/*
			*	Setup global events (bad)
			*/
			input.event.tick.reg(&TestEngine::updateProjection, this);
			event.tick.reg(&TestEngine::moveSelected, this);
			
			/*
			*	Setup GUIs
			*/
			gui1.setGlobalFont("fonts/DejaVuSans.ttf");

			loadWidgets(gui1, gui2);

			/*
			*	Setup shaders
			*/
			vertexShader.loadFromFile("shader.vert");
			vertexShader.compile();

			fragmentShader.loadFromFile("shader.frag");
			fragmentShader.compile();

			program.attachShader(vertexShader);
			program.attachShader(fragmentShader);
			program.linkProgram();

			program.addAttribute("position");
			program.addAttribute("uv");
			program.addAttribute("normal");

			program.addUniform("MVP");
			program.addUniform("M");
			//program.addUniform("N");
			
			program.addUniform("numDirectionalLights");
			program.addUniform("numPointLights");
			program.addUniform("numSpotLights");

			program.addUniform("directionalLight[0].base.color");
			program.addUniform("directionalLight[0].base.diffuseIntensity");
			program.addUniform("directionalLight[0].base.ambientIntensity");
			program.addUniform("directionalLight[0].direction");
			program.addUniform("directionalLight[1].base.color");
			program.addUniform("directionalLight[1].base.diffuseIntensity");
			program.addUniform("directionalLight[1].base.ambientIntensity");
			program.addUniform("directionalLight[1].direction");
			
			program.addUniform("pointLight[0].base.color");
			program.addUniform("pointLight[0].base.diffuseIntensity");
			program.addUniform("pointLight[0].base.ambientIntensity");
			program.addUniform("pointLight[0].position");
			program.addUniform("pointLight[0].atten.constant");
			program.addUniform("pointLight[0].atten.linear");
			program.addUniform("pointLight[0].atten.exp");
			program.addUniform("pointLight[1].base.color");
			program.addUniform("pointLight[1].base.diffuseIntensity");
			program.addUniform("pointLight[1].base.ambientIntensity");
			program.addUniform("pointLight[1].position");
			program.addUniform("pointLight[1].atten.constant");
			program.addUniform("pointLight[1].atten.linear");
			program.addUniform("pointLight[1].atten.exp");
			
			program.addUniform("spotLight[0].base.base.color");
			program.addUniform("spotLight[0].base.base.diffuseIntensity");
			program.addUniform("spotLight[0].base.base.ambientIntensity");
			program.addUniform("spotLight[0].base.position");
			program.addUniform("spotLight[0].base.atten.constant");
			program.addUniform("spotLight[0].base.atten.linear");
			program.addUniform("spotLight[0].base.atten.exp");
			program.addUniform("spotLight[0].direction");
			program.addUniform("spotLight[0].cutoff");
			program.addUniform("spotLight[1].base.base.color");
			program.addUniform("spotLight[1].base.base.diffuseIntensity");
			program.addUniform("spotLight[1].base.base.ambientIntensity");
			program.addUniform("spotLight[1].base.position");
			program.addUniform("spotLight[1].base.atten.constant");
			program.addUniform("spotLight[1].base.atten.linear");
			program.addUniform("spotLight[1].base.atten.exp");
			program.addUniform("spotLight[1].direction");
			program.addUniform("spotLight[1].cutoff");
			
			program.addUniform("materialDiffuse");
			program.addUniform("materialAmbient");
			program.addUniform("materialSpecular");
			program.addUniform("materialShine");
			program.addUniform("cameraPosition");
			program.addUniform("sampler");
			
			graphics.program = &program;

			/*
			*	Setup Meshes (normally this is bad! need a mesh manager!)
			*/
			mesh = loadMesh("box.obj");

			/*
			*	Setup Actor Context
			*/
			actorManager.context.add(graphics);
			actorManager.context.add(input);

			/*
			*	Setup Actors
			*/
			actor = new Actor(actorManager, "actor");

			//actor->setTransformation(glm::scale(actor->getTransformation(), glm::vec3(10)));
			//actor->setTransformation(glm::scale(actor->getTransformation(), glm::vec3(.01)));
			//actor->setTransformation(glm::translate(actor->getTransformation(), glm::vec3(0, 0, -2)));

			/*
			*	setup components
			*/
			ActorGraphicsComponent& actorGraphics = *new ActorGraphicsComponent(*actor, "Graphics Component");
			actorGraphics.mesh = mesh;
			
			/*
			*	setup camera
			*/
			Actor* targetTest = NULL;
			camera.setPosition(glm::vec3(0, 0, -3));
			//camera.rotate(glm::vec3(glm::pi<float>()/2, 0, 0));
			camera.target = targetTest;
			
			ambientLight = new DirectionalLight();
			ambientLight->direction = glm::normalize(glm::vec3(0, 0, 1));
			ambientLight->color = glm::vec3(1, 0, 1);
			ambientLight->diffuseIntensity = 0;
			ambientLight->ambientIntensity = .1;
			graphics.lights.push_back(ambientLight);
			
			directionalLight = new DirectionalLight();
			directionalLight->direction = glm::normalize(glm::vec3(0, 0, 1));
			directionalLight->color = glm::vec3(1, 1, 1);
			directionalLight->diffuseIntensity = .1;
			directionalLight->ambientIntensity = 0;
			graphics.lights.push_back(directionalLight);
			
			pointLight = new PointLight();
			pointLight->position = glm::vec3(0, 5, 0);
			pointLight->color = glm::vec3(0, .6, 0);
			pointLight->diffuseIntensity = .8;
			pointLight->ambientIntensity = 0;
			pointLight->attenuation.constant = 1;
			pointLight->attenuation.linear = .4;
			pointLight->attenuation.exp = .2;
			graphics.lights.push_back(pointLight);
			
			SpotLight* spotLight = new SpotLight();
			spotLight->direction = glm::normalize(glm::vec3(-1, 1, 0));
			spotLight->position = glm::vec3(3, -4, 0);
			spotLight->color = glm::vec3(0, .8, .8);
			spotLight->diffuseIntensity = 16;
			spotLight->ambientIntensity = 0;
			spotLight->attenuation.constant = 1;
			spotLight->attenuation.linear = .4;
			spotLight->attenuation.exp = .2;
			spotLight->cutoff = glm::pi<float>()/32;
			spotLights.push_back(spotLight);
			graphics.lights.push_back(spotLight);
			
			auto& cameraInput = *new CameraInputControl(camera, "Camera Input Control", *this, window);
		} //TestEngine

		virtual ~TestEngine()
		{
			if (mesh)
			{
				delete mesh;
			} //if
			
			for (Light* light : graphics.lights)
			{
				delete light;
			} //for
		} //~TestEngine
}; //TestEngine
