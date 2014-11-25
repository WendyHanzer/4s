#pragma once

#include "menu.h"
#include "../Utils/Event.h"

class MainMenu : public Menu
{
	Event<void> eventStartGame;
	Event<void> eventRestartGame;
	Event<void> eventChangePaddleShape;
	Event<void> eventQuit;
	
	void startGame(const tgui::Callback& callback)
	{
		//play against AI?
		//play against player?
		
		eventStartGame.exec();
	} //quit
	void restartGame(const tgui::Callback& callback)
	{
		//restart game?
		
		eventRestartGame.exec();
	} //quit
	void changePaddleShape(const tgui::Callback& callback)
	{
		//change paddle shape?
			//btBoxShape
			//btCylinderShape
			
		eventChangePaddleShape.exec();
	} //quit
	void quit(const tgui::Callback& callback)
	{
		//exit
		eventQuit.exec();
	} //quit

	public:
	
		MainMenu(Layer& layer) : Menu(layer)
		{
			const int OFF = 100;
			const int SPACING = 30;
			
			tgui::Button::Ptr startGame;
			startGame->load("widgets/Black.conf");
			startGame->setSize(128, SPACING);
			startGame->setPosition(panel->getSize()/2, OFF + SPACING*0);
			startGame->setText("Start New Game");
			panel->add(startGame);
			startGame->bindCallbackEx(std::bind(&MainMenu::startGame, this, std::placeholders::_1), tgui::Button::LeftMouseClicked);
			
			tgui::Button::Ptr restartGame;
			restartGame->load("widgets/Black.conf");
			restartGame->setSize(128, SPACING);
			restartGame->setPosition(panel->getSize()/2, OFF + SPACING*1);
			restartGame->setText("Restart Game");
			panel->add(restartGame);
			restartGame->bindCallbackEx(std::bind(&MainMenu::restartGame, this, std::placeholders::_1), tgui::Button::LeftMouseClicked);
			
			tgui::Button::Ptr changePaddleShape;
			changePaddleShape->load("widgets/Black.conf");
			changePaddleShape->setSize(128, SPACING);
			changePaddleShape->setPosition(panel->getSize()/2, OFF + SPACING*2);
			changePaddleShape->setText("Change Paddle Shape");
			panel->add(changePaddleShape);
			changePaddleShape->bindCallbackEx(std::bind(&MainMenu::changePaddleShape, this, std::placeholders::_1), tgui::Button::LeftMouseClicked);
		
			tgui::Button::Ptr quit;
			quit->load("widgets/Black.conf");
			quit->setSize(128, SPACING);
			quit->setPosition(panel->getSize()/2, OFF + SPACING*3);
			quit->setText("Quit");
			panel->add(quit);
			quit->bindCallbackEx(std::bind(&MainMenu::quit, this, std::placeholders::_1), tgui::Button::LeftMouseClicked);
		} //MainMenu
		
}; //MainMenu
