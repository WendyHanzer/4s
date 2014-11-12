#pragma once

#include "../Input/InputSystem.h"
#include "../Entity/Actor/Actor.h"
#include "../Entity/Actor/Component/AI/Command/ActorCommandFollow.h"
#include "../Entity/Actor/Component/AI/Command/ActorCommandStop.h"

#include "Player.h"

class Human : public Player
{
	public:
	
		enum CONTROL_MODE
		{
			CONTROL_INVALID = 0,
			CONTROL_MOUSE,
			CONTROL_KEYBOARD
		}; //CONTROL_MODE
		
		CONTROL_MODE mode;

	private:
	
		Actor& mouse;
		Actor& keyboard;
	
		Event<const long double&>::id actionTick;
		
		void onTick(const long double& delta)
		{
			static const float SENSITIVITY = 10;
			
			glm::vec3 move(0, 0, 0);
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				move.x -= SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				move.x += SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				move.z -= SENSITIVITY*delta;
			} //if
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				move.z += SENSITIVITY*delta;
			} //if
			
			glm::mat4 trans = keyboard.getTransformation();
			
			trans[3][0] += move.x;
			trans[3][1] += move.y;
			trans[3][2] += move.z;
			
			keyboard.setTransformation(trans);
		} //onTick
		
	public:
	
		Human(const std::string& name, Score& score, Paddle& paddle, Goal& goal, Actor& puck, Engine& engine, Actor& mouse, Actor& keyboard, const CONTROL_MODE& mode) :
		Player(name, score, paddle, goal, puck, engine),
		mode(CONTROL_INVALID),
		mouse(mouse),
		keyboard(keyboard)
		{
			actionTick = engine.event.tick.reg(&Human::onTick, this);
			
			actionTick->enable = false;
			
			setControlMode(mode);
		} //Human
		
		void setControlMode(const CONTROL_MODE& mode)
		{
			if (this->mode == mode)
			{
				return;
			} //if
			
			new ActorCommandStop(paddle);
			
			switch (this->mode)
			{
				case CONTROL_KEYBOARD:
					actionTick->enable = false;
					mouse.setTransformation(keyboard.getTransformation());
				
					break;
				case CONTROL_MOUSE:
					keyboard.setTransformation(mouse.getTransformation());
					
					break;
				default:
					break;
			} //switch
			
			this->mode = mode;
			
			switch (this->mode)
			{
				case CONTROL_KEYBOARD:
					actionTick->enable = true;
					new ActorCommandFollow(paddle, "follow", mouse, -1, .1);
				
					break;
				case CONTROL_MOUSE:
					new ActorCommandFollow(paddle, "follow", keyboard, -1, .1);
					
					break;
				default:
					break;
			} //switch
		} //setControlMode
		
		virtual ~Human()
		{
			actionTick->unreg();
		} //Human
}; //Human
