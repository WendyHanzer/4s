#include "InputSystem.h"

InputSystem::InputSystem(Engine& engine, sf::RenderWindow& window, tgui::Gui& gui0, tgui::Gui& gui1, tgui::Gui& gui2) :
	System(engine),
	gui0(gui0),
	gui1(gui1),
	gui2(gui2),
	window(window)
{
} //GraphicsSystem

InputSystem::~InputSystem()
{
	destroy();
	
	if (window.isOpen())
	{
		window.close();
	} //if
} //~InputSystem

void InputSystem::onTick(const long double& delta)
{
	if (!window.isOpen())
	{
		engine.stop();
		return;
	} //if

	while (window.pollEvent(inputResponse))
	{
		System::onTick(delta);
		
		gui0.handleEvent(inputResponse);
		gui1.handleEvent(inputResponse);
		gui2.handleEvent(inputResponse);
	} //while
	
	while (gui0.pollCallback(callback))
	{
	} //while
	while (gui1.pollCallback(callback))
	{
	} //while
	while (gui2.pollCallback(callback))
	{
	} //while
} //onTick
