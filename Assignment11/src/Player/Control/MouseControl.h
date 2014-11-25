#pragma once

#include "../../Engine/Engine.h"
#include "../../Graphics/GraphicsSystem.h"
#include "../../Input/InputSystem.h"

#include "../../Entity/Actor/Actor.h"
#include "../../Entity/Actor/ActorManager.h"

class MouseControl : public PulseBehavior<>
{
	public:
	
		Actor& mouse;

	private:

		sf::Window& window;
		InputSystem& input;
		sf::Vector2<int> center;
	
	protected:
	
		virtual void onTick(const long double& delta)
		{
			switch (input.inputResponse.type)
			{
				case sf::Event::MouseMoved:
					
				
					mouse.setTransformation(glm::translate(mouse.getTransformation(), glm::vec3(
						(input.inputResponse.mouseMove.x - center.x)*delta*.03, 0,
						(input.inputResponse.mouseMove.y - center.y)*delta*.03)));
					
					actionTick->enable = false;
					sf::Mouse::setPosition(center, window);
					actionTick->enable = true;
						
					break;
				default:
					break;
			} //switch
		} //onInput
		
		virtual void onStart()
		{
			window.setMouseCursorVisible(false);
			
			MouseControl::onStart();
		} //onStart
		
		virtual void onStop()
		{
			window.setMouseCursorVisible(true);
			
			MouseControl::onStop();
		} //onStop

	public:
	
		MouseControl(InputSystem& input, ActorManager& actorManager, sf::Window& window) : PulseBehavior(input.event),
		mouse(*new Actor(actorManager, "mouse")),
		window(window),
		input(input),
		center(window.getSize().x/2, window.getSize().y/2)
		{
		} //MouseControl
		
		~MouseControl()
		{
			destroy();
		} //~MouseControl
}; //MouseControl
