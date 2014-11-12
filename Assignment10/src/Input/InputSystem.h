#pragma once

#include <GL/glew.h>
#include <TGUI/TGUI.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../Engine/System/System.h"

class InputSystem : public System
{
	private:
	
		tgui::Callback callback;
		
		tgui::Gui& gui0;		//menu
		
		tgui::Gui& gui1;		//in front
		//scene
		tgui::Gui& gui2;		//in back
	
	public:
	
		sf::Event inputResponse;

		sf::RenderWindow& window;

		InputSystem(Engine& engine, sf::RenderWindow& window, tgui::Gui& gui0, tgui::Gui& gui1, tgui::Gui& gui2);
		virtual ~InputSystem();

	protected:

		virtual void onTick(const long double& delta);

}; //InputSystem
