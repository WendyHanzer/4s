#pragma once

#include "../Entity/Actor/ActorManager.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Player/Control/MouseControl.h"
#include "../Player/Control/CameraControl.h"

class Menu;
class Layer
{
	public:
	
		tgui::Gui& gui;
		Menu* activeMenu;
		
	private:
		
		ActorManager& actorManager;
		GraphicsSystem& graphics;
		MouseControl& mouse;
		CameraControl& camera;
		
	public:
		
		Layer(tgui::Gui& gui, ActorManager& actorManager, GraphicsSystem& graphics, MouseControl& mouse, CameraControl& camera) :
			gui(gui),
			activeMenu(NULL), 
			actorManager(actorManager),
			graphics(graphics),
			mouse(mouse),
			camera(camera)
		{
		} //Layer
		
		/*
		*	this stuff should be handled in a gamestate controller
		*	pause/unpause game
		*
		*	There is no time to do this correctly, which sucks
		*/
		void stop()
		{
			actorManager.stop();
			graphics.stop();
			graphics.start();
			
			mouse.stop();
			camera.stop();
		} //stop
		
		void start()
		{
			actorManager.start();
			
			mouse.start();
			camera.start();
			
			if (activeMenu != NULL)
			{
				gui.removeAllWidgets();
				activeMenu = NULL;
			} //if
		} //start
		
}; //Layer

class Menu
{
	private:
		Layer& layer;
		
		void transition()
		{
			layer.activeMenu = NULL;
			layer.gui.remove(panel);
		} //transition
		
	public:
	
		tgui::Panel::Ptr panel;
		
		Menu(Layer& layer) :
		layer(layer)
		{
			panel->setBackgroundColor(sf::Color::Black);
			panel->setTransparency((unsigned char)128);
			
			//should be window size or something
			panel->setSize(800, 600);
		} //Menu
		
		void show()
		{
			if (layer.activeMenu == this) { return; }
			
			if (layer.activeMenu != NULL)
			{
				layer.activeMenu->transition();
			} //if
			else
			{
				layer.stop();
			} //else
			
			layer.activeMenu = this;
			layer.gui.add(panel, "panel");
		} //show
		
		void hide()
		{
			if (layer.activeMenu != this) { return; }
			
			transition();
			
			layer.start();
		} //hide
		
		bool isVisible() { return layer.activeMenu == this; }
		
}; //Menu
