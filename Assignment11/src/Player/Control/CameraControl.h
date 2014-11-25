#pragma once

#include "../../Engine/Engine.h"

#include "../../Engine/Entity/Component/Component.h"
#include "../../Entity/Actor/Abstract/Camera.h"

class CameraControl : public PulseBehavior<>
{
	public:
	
		Camera& camera;
	
	protected:
	
		virtual void onTick(const long double& delta)
		{
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
			
			camera.translate(move);
			camera.rotate(euler);
			
			CameraControl::onTick(delta);
		} //onTick
		
		void onDestroy()
		{
			delete this;
		} //onDestroy

	public:
	
		CameraControl(Camera& camera, Engine& engine) : PulseBehavior(engine.event), camera(camera)
		{
			camera.event.destroy.reg(&CameraControl::onDestroy, this);
		} //CameraControl
		
		~CameraControl()
		{
			destroy();
		} //~CameraControl
}; //CameraControl
