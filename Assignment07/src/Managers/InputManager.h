#pragma once

#include <SFML/String.hpp>
#include <SFML/Keyboard.hpp>
#include <SFML/Mouse.hpp>
#include <SFML/Event.hpp>

#include "Event.h"
#include "Engine.h"

class InputManager
{ /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			//std::vector<Scene*> scenes;

			Engine* engine;

			void onDestroy();
			//this calls destructor when engine is destroyed

			void onTick();
			//runs under SceneManager::eventTick
			//runs eventRender

/*		Constructors
*		----------------------------
*/
			InputManager(Engine* engine);
			~InputManager();
/*
*****************************************************************
*
*/	public: /*
*
*
*		Fields
*		----------------------------------
*/
			Event<void> eventStart;
			Event<void> eventStop;
			Event<void> eventDestroy;

			struct
			{
				struct
				{
					Event<void> Closed
					Event<void> Resized
						//Event.Size
					Event<void> LostFocus
					Event<void> GainedFocus
				} Window;
				struct
				{
					Event<void> WheelMoved;
						//Event.MouseWheel.Delta contains the mouse wheel move (positive if forward, negative if backward)
					Event<void> AnyButtonPressed;
						map<sf::Mouse::Button, Event<void>*> ButtonPressed;
						void initButtonPressed(sf::Mouse::Button button);
					Event<void> AnyButtonReleased;
						map<sf::Mouse::Button, Event<void>*> ButtonReleased;
						void initButtonReleased(sf::Mouse::Button button);
						//Event.MouseButton.Button contains the buttons that is pressed / released
						//Event.MouseButton.X contains the current X position of the mouse cursor, in local coordinates
						//Event.MouseButton.Y contains the current Y position of the mouse cursor, in local coordinates
					Event<void> Moved;
						//Event.MouseMove.X contains the new X position of the mouse cursor, in local coordinates
						//Event.MouseMove.Y contains the new Y position of the mouse cursor, in local coordinates
					Event<void> Entered;
					Event<void> Left;
				} Mouse;
				struct
				{
					Event<void> AnyTextEntered;
						map<sf::Uint32, Event<void>*> TextEntered;
						void initTextEntered(sf::Uint32 unicode);
						//Event.Text.Unicode
					Event<void> AnyKeyPressed;
						map<sf::Keyboard::Key, Event<void>*> KeyPressed;
						void initKeyPressed(sf::Keyboard::Key key);
					Event<void> AnyKeyReleased;
						map<sf::Keyboard::KeyButton, Event<void>*> KeyReleased;
						void initKeyReleased(sf::Keyboard::Key key);
						//Event.Key.Code contains the code of the key that was pressed / released
						//Event.Key.Alt tells whether or not Alt key is pressed
						//Event.Key.Control tells whether or not Control key is pressed
						//Event.Key.Shift tells whether or not Shift key is pressed
				} Keyboard;
			} Event;


/*
*		Properties
*		----------------------------------
*/


/*
*****************************************************************/

}; //InputManager
