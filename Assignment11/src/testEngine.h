#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <SFML/Audio.hpp>

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
					break;
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
			
			//sf::Mouse::setPosition(prevMousePosition, window);
			
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
	/*
	*	engine stuff
	*/
	sf::RenderWindow window;

	tgui::Gui gui0;
	tgui::Gui gui1;
	tgui::Gui gui2;		//menu GUI
	
	tgui::Label::Ptr scoreDisplay;
	tgui::Label::Ptr clockDisplay;
	tgui::Label::Ptr lifeDisplay;
	tgui::Label::Ptr levelDisplay;
	tgui::EditBox::Ptr initialInput;
	tgui::Button::Ptr initialInputEnter;
	tgui::Panel::Ptr scorePanel;
	tgui::Label::Ptr highScore[21];

	ActorManager& actorManager;
	
	Camera& camera;

	GraphicsSystem& graphics;
	InputSystem& input;
	PhysicsSystem& physics;

	Shader vertexShader;
	Shader fragmentShader;
	ShaderProgram program;
	
	/*
	*	Resources
	*/
	Mesh* ball;
	Mesh* box;
	Mesh* knob;
	
	Texture* stone;
	Texture* lava;
	Texture* red;
	Texture* green;
	Texture* black;
	
	sf::Music gameLoop;
	sf::Music lose;
	sf::Music win;
	
	/*
	*	Game Objects
	*
	*		some of these should be in a level struct
	*/
	Actor* player;
	Actor* bottomKnob;
	Actor* rightKnob;
	
	DirectionalLight* ambientLight;
	PointLight* pointLight;
	std::vector<SpotLight*> spotLights;		//can use to mark start/end
	DirectionalLight* directionalLight;
	
	std::vector<Actor*> blocks;				//actors in the level
	glm::vec3 startPosition;				//start position
	glm::vec3 goal;							//end position
	
	bool paused;
	bool inLevel;
	bool inEndMenu;
	
	/*
	*	Game Data
	*/
	int startLives;
	int lives;
	int level;
	int totalScore;
	int lifeBonus;	//how many bonus points to give per
					//remaining life upon winning
	
	/*
	*	Game Scores
	*/
	//base/time*multiplier = score
	double base;
	double multiplier;
	double currentTime;
	
	/*
	*	Settings
	*/
	float sensitivity;
	
	//used for scaling between blocks so there
	//are no spaces
	float scaling;
	
	/*
	*	this primarily handles hotkeys
	*/
	void handleInput(const long double& delta)
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
					//should bring up a menu and pause the game
					case sf::Keyboard::Escape:
						stop();
						break;
					default:
						break;
				} //switch

				break;
				
			default:
				break;
		} //switch
	} //handleInput

	/*
	*	this primarily handles held keys
	*/
	void handleHeld(const long double& delta)
	{
		if (paused || !inLevel)
		{
			return;
		} //if
		
		/*
		*	calculate key presses
		*/
		glm::vec3 rotation;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rotation.y -= sensitivity*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rotation.y += sensitivity*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			rotation.x += sensitivity*delta;
		} //if
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			rotation.x -= sensitivity*delta;
		} //if
		
		if (glm::length(rotation) == 0)
		{
			return;
		} //if
		
		/*
		*	apply rotations with a constraint
		*/
		static const float constraint = glm::pi<float>()/4;
		
		if (glm::pitch(graphics.worldRotationX) + rotation.x > constraint)
		{
			graphics.worldRotationX = glm::angleAxis(constraint, glm::vec3(1, 0, 0));
		} //if
		else if (glm::pitch(graphics.worldRotationX) + rotation.x < -constraint)
		{
			graphics.worldRotationX = glm::angleAxis(-constraint, glm::vec3(1, 0, 0));
		} //if
		else
		{
			graphics.worldRotationX = glm::rotate(graphics.worldRotationX, rotation.x, glm::vec3(1, 0, 0));
		} //else
		
		if (glm::roll(graphics.worldRotationY) + rotation.y > constraint)
		{
			graphics.worldRotationY = glm::angleAxis(constraint, glm::vec3(0, 0, 1));
		} //if
		else if (glm::roll(graphics.worldRotationY) + rotation.y < -constraint)
		{
			graphics.worldRotationY = glm::angleAxis(-constraint, glm::vec3(0, 0, 1));
		} //if
		else
		{
			graphics.worldRotationY = glm::rotate(graphics.worldRotationY, rotation.y, glm::vec3(0, 0, 1));
		} //else
		
		/*
		*	synchronize knobs to board
		*/
		glm::mat4 knobRotation = glm::toMat4(graphics.worldRotationX);
		glm::mat4 knobPosition = bottomKnob->getTransformation();
		knobRotation[3][0] = knobPosition[3][0];
		knobRotation[3][1] = knobPosition[3][1];
		knobRotation[3][2] = knobPosition[3][2];
		bottomKnob->setTransformation(knobRotation);
		
		knobRotation = glm::toMat4(graphics.worldRotationY);
		knobPosition = rightKnob->getTransformation();
		knobRotation[3][0] = knobPosition[3][0];
		knobRotation[3][1] = knobPosition[3][1];
		knobRotation[3][2] = knobPosition[3][2];
		rightKnob->setTransformation(knobRotation);
		
		/*
		*	synchronize gravity vector to board
		*/
		glm::vec3 unitVector(0, 1, 0);
		unitVector = graphics.worldRotationX*graphics.worldRotationY*unitVector;
		physics.setGravityDirection(btVector3(-unitVector.x, unitVector.y, -unitVector.z));
	} //handleHeld

	/*
	*	GUI
	*/
	void quitCallback(const tgui::Callback& callback)
	{
		stop();
	}
	void startCallback(const tgui::Callback& callback)
	{
		unpause();
	}
	void pauseCallback(const tgui::Callback& callback)
	{
		pause();
	}
	void replayCallback(const tgui::Callback& callback)
	{
		if (scoreboard != NULL)
		{
			gui1.remove(initialInput);
			gui1.remove(initialInputEnter);
			
			delete scoreboard;
			scoreboard = NULL;
		} //if
		
		if (inEndMenu)
		{
			gui1.remove(scorePanel);
		} //if
		
		replay();
	}
	
	void initialInputEnterCallback(const tgui::Callback& callback)
	{
		static const int MAX_NAME_LENGTH = 12;
	
		if (initialInput->getText() != string(""))
		{
			initialInput->setText(string(initialInput->getText()).substr(0, MAX_NAME_LENGTH));
		
			gui1.remove(initialInput);
			gui1.remove(initialInputEnter);

			finishHighScores(initialInput->getText());
		} //if
	}
	void getUserName()
	{
		initialInput->setText("");
		
		gui1.add(initialInput);
		gui1.add(initialInputEnter);
		initialInputEnter->setText("Enter");
	} //getUserName
	
	void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2)
	{
		tgui::Picture::Ptr picture(gui2);
		picture->load("images/3d_chinese_dragon-1920x1080.jpg");

		tgui::Button::Ptr quit(gui);
		quit->load("widgets/Black.conf");
		quit->setSize(128, 30);
		quit->setPosition(window.getSize().x - 256 - 15, 45);
		quit->setText("Quit");
		quit->bindCallbackEx(&TestEngine::quitCallback, this, tgui::Button::LeftMouseClicked);
		
		tgui::Button::Ptr start(gui);
		start->load("widgets/Black.conf");
		start->setSize(128, 30);
		start->setPosition(window.getSize().x - 256 - 15, 15);
		start->setText("Start");
		start->bindCallbackEx(&TestEngine::startCallback, this, tgui::Button::LeftMouseClicked);
		
		tgui::Button::Ptr pause(gui);
		pause->load("widgets/Black.conf");
		pause->setSize(128, 30);
		pause->setPosition(window.getSize().x - 128 - 15, 15);
		pause->setText("Pause");
		pause->bindCallbackEx(&TestEngine::pauseCallback, this, tgui::Button::LeftMouseClicked);
		
		tgui::Button::Ptr replay(gui);
		replay->load("widgets/Black.conf");
		replay->setSize(128, 30);
		replay->setPosition(window.getSize().x - 128 - 15, 45);
		replay->setText("Replay");
		replay->bindCallbackEx(&TestEngine::replayCallback, this, tgui::Button::LeftMouseClicked);
		
		gui.add(scoreDisplay, "score");
		gui.add(clockDisplay, "clock");
		gui.add(lifeDisplay, "lives");
		gui.add(levelDisplay, "level");
		
		tgui::Label::Ptr scoreLabel(gui);
		scoreLabel->load("widgets/Black.conf");
		scoreLabel->setText("Score");
		scoreLabel->setPosition(60, 15);
		scoreLabel->setTextColor(sf::Color(200, 200, 200));
		scoreLabel->setTextSize(20);
		
		scoreDisplay->load("widgets/Black.conf");
		scoreDisplay->setText("0");
		scoreDisplay->setPosition(150, 15);
		scoreDisplay->setTextColor(sf::Color(200, 200, 200));
		scoreDisplay->setTextSize(20);
		
		tgui::Label::Ptr clockLabel(gui);
		clockLabel->load("widgets/Black.conf");
		clockLabel->setText("Time");
		clockLabel->setPosition(window.getSize().x/2 - 90, 15);
		clockLabel->setTextColor(sf::Color(200, 200, 200));
		clockLabel->setTextSize(20);
		
		clockDisplay->load("widgets/Black.conf");
		clockDisplay->setText("0:00");
		clockDisplay->setPosition(window.getSize().x/2, 15);
		clockDisplay->setTextColor(sf::Color(200, 200, 200));
		clockDisplay->setTextSize(20);
		
		tgui::Label::Ptr lifeLabel(gui);
		lifeLabel->load("widgets/Black.conf");
		lifeLabel->setText("Lives");
		lifeLabel->setPosition(60, 45);
		lifeLabel->setTextColor(sf::Color(200, 200, 200));
		lifeLabel->setTextSize(20);
		
		lifeDisplay->load("widgets/Black.conf");
		lifeDisplay->setText("0");
		lifeDisplay->setPosition(150, 45);
		lifeDisplay->setTextColor(sf::Color(200, 200, 200));
		lifeDisplay->setTextSize(20);
		
		tgui::Label::Ptr levelLabel(gui);
		levelLabel->load("widgets/Black.conf");
		levelLabel->setText("Level");
		levelLabel->setPosition(60, 75);
		levelLabel->setTextColor(sf::Color(200, 200, 200));
		levelLabel->setTextSize(20);
		
		levelDisplay->load("widgets/Black.conf");
		levelDisplay->setText("0");
		levelDisplay->setPosition(150, 75);
		levelDisplay->setTextColor(sf::Color(200, 200, 200));
		levelDisplay->setTextSize(20);
		
		initialInput->load("widgets/Black.conf");
		initialInput->setPosition(window.getSize().x/2 - 280, window.getSize().y/2);
		initialInput->setSize(300, 30);
		
		initialInputEnter->load("widgets/Black.conf");
		initialInputEnter->setSize(128, 30);
		initialInputEnter->setPosition(window.getSize().x/2 + 25, window.getSize().y/2);
		initialInputEnter->setText("");
		initialInputEnter->bindCallbackEx(&TestEngine::initialInputEnterCallback, this, tgui::Button::LeftMouseClicked);
		
		scorePanel->setSize(window.getSize().x/2, window.getSize().y/2);
		scorePanel->setPosition(window.getSize().x/4, 160);
		scorePanel->setBackgroundColor(sf::Color(32, 32, 32, 128));
		
		for (int i = 0; i < 10; ++i)
		{
			scorePanel->add(highScore[i]);
			
			highScore[i]->load("widgets/Black.conf");
			highScore[i]->setText("");
			highScore[i]->setPosition(220, 110 + 25*i);
			highScore[i]->setTextColor(sf::Color(200, 200, 200));
			highScore[i]->setTextSize(20);
		} //for
		
		for (int i = 10; i < 20; ++i)
		{
			scorePanel->add(highScore[i]);
			
			highScore[i]->load("widgets/Black.conf");
			highScore[i]->setText("");
			highScore[i]->setPosition(550, 110 + 25*(i - 10));
			highScore[i]->setTextColor(sf::Color(200, 200, 200));
			highScore[i]->setTextSize(20);
		} //for
		
		scorePanel->add(highScore[20]);
		highScore[20]->load("widgets/Black.conf");
		highScore[20]->setText("High Scores");
		highScore[20]->setPosition(355, 40);
		highScore[20]->setTextColor(sf::Color(200, 200, 200));
		highScore[20]->setTextSize(20);
	} //loadWidgets
	
	struct ScoreBoard
	{
		int scores[11];
		int count;
		int newScore;
		string names[11];
		
		ScoreBoard()
		{
			count = 0;
			
			for (int i = 0; i < 11; ++i)
			{
				scores[i] = 0;
			} //for
			
			newScore = 10;
		} //ScoreBoard
	}; //ScoreBoard
	
	//should probably use std::bind here to clean things up a bit
	ScoreBoard* scoreboard;
	
	ScoreBoard* getHighScores()
	{
		if (totalScore == 0)
		{
			return NULL;
		} //if
	
		ScoreBoard& board = *new ScoreBoard();
		
		/*
		*	load scores
		*/
		ifstream scoreFile;
		scoreFile.open("scores.txt");
		while (scoreFile.good() && board.count < 10)
		{
			getline(scoreFile, board.names[board.count]);
			scoreFile >> board.scores[board.count];
			scoreFile.get();
			
			if (board.scores[board.count] == 0)
			{
				--board.count;
			} //if
			
			++board.count;
		} //while
		scoreFile.close();
		
		/*
		*	shift everything down to make room for our score
		*/
		int position = board.count;
		while (position > 0 && board.scores[position - 1] < totalScore)
		{
			board.names[position] = board.names[position - 1];
			board.scores[position] = board.scores[position - 1];
			--position;
		} //while
		
		/*
		*	if a position was found, the score is a new high score
		*/
		if (position < 10)
		{
			board.newScore = position;
			
			if (board.count < 10)
			{
				++board.count;
			} //if
		
			board.scores[position] = totalScore;
			board.names[position] = "Unknown";
		} //if
		else
		{
			delete &board;
		} //else
		
		return &board;
	} //getHighScores
	
	void updateHighScores(ScoreBoard* board, const string& name)
	{
		board->names[board->newScore] = name;
		
		ofstream outScore;
		outScore.open("scores.txt");
		for (int i = 0; i < board->count; ++i)
		{
			outScore << board->names[i] << endl;
			outScore << board->scores[i] << endl;
		} //for
		outScore.close();
	} //updateHighScores
	
	void finishHighScores(const string& name)
	{
		updateHighScores(scoreboard, name);
		delete scoreboard;
		scoreboard = NULL;
		
		endGameMenu();
	} //finishHighScores
	
	void displayHighScores()
	{
		string names[10];
		string scores[10];
		int count = 0;
	
		ifstream scoreFile;
		scoreFile.open("scores.txt");
		while (scoreFile.good() && count < 10)
		{
			getline(scoreFile, names[count]);
			scoreFile >> scores[count++];
			scoreFile.get();
			if (scores[count - 1] == "")
			{
				--count;
			} //if
		} //while
		scoreFile.close();
		
		gui1.add(scorePanel);
		
		highScore[20]->setText("High Scores");
		
		stringstream ss;
		for (int i = 0; i < count; ++i)
		{
			ss << (i + 1);
			highScore[i]->setText(ss.str() + string(". ") + names[i]);
			ss.str("");
		} //for
		
		for (int i = 10; i < 20; ++i)
		{
			highScore[i]->setText(scores[i - 10]);
		} //for
	} //displayHighScores
	
	void endGameMenu()
	{
		inEndMenu = true;
	
		displayHighScores();
	
		if (lives > 0)
		{
			//play victory music and display you win?
			//display replay as replay?
		} //if
		else
		{
			//display replay as try again?
		} //else
	} //endGameMenu
	
	/*
	*	Game Methods
	*/
	void endGame(bool won)
	{
		/*
		*	handle high score
		*/
		if (lives > 0)
		{
			totalScore += (lives - 1)*lifeBonus;
		} //if
		scoreboard = getHighScores();
		
		if (gameLoop.getStatus() == sf::SoundSource::Status::Playing)
		{
			gameLoop.stop();
		} //if
		
		if (won)
		{
			if (win.getStatus() != sf::SoundSource::Status::Playing)
			{
				win.play();
			} //if
		} //if
		else
		{
			if (lose.getStatus() != sf::SoundSource::Status::Playing)
			{
				lose.play();
			} //if
		} //else
		
		if (scoreboard != NULL)
		{
			getUserName();
		} //if
		else
		{
			endGameMenu();
		} //else
	} //endGame
	
	void replay()
	{
		unpause();
		endLevel(false);
	
		lives = startLives;
		level = 0;
		totalScore = 0;
		updateLives();
		updateScore();
		
		nextLevel();
	} //replay
	
	void setInLevel(bool inLevel)
	{
		if (this->inLevel == inLevel)
		{
			return;
		} //if
		
		this->inLevel = inLevel;
		
		if (!inLevel)
		{
			((ActorGraphicsComponent&)(player->components[typeid(GraphicsSystem)])).stop();
			((ActorGraphicsComponent&)(rightKnob->components[typeid(GraphicsSystem)])).stop();
			((ActorGraphicsComponent&)(bottomKnob->components[typeid(GraphicsSystem)])).stop();
		} //if
		else
		{
			((ActorGraphicsComponent&)(player->components[typeid(GraphicsSystem)])).start();
			((ActorGraphicsComponent&)(rightKnob->components[typeid(GraphicsSystem)])).start();
			((ActorGraphicsComponent&)(bottomKnob->components[typeid(GraphicsSystem)])).start();
			
			((ActorPhysicsComponent&)(player->components[typeid(PhysicsSystem)])).stop();
			((ActorPhysicsComponent&)(player->components[typeid(PhysicsSystem)])).start();
			((ActorPhysicsComponent&)(player->components[typeid(PhysicsSystem)])).reset();
		} //else
	} //setInLevel
	
	void pause()
	{
		if (paused)
		{
			return;
		} //if
		
		paused = true;
		
		physics.stop();
	} //pause
	
	void unpause()
	{
		if (!paused)
		{
			return;
		} //if
		
		paused = false;
	
		physics.start();
	} //unpause()
	
	void createBlock(const glm::vec3& position, Texture* texture)
	{
		Actor& block = *new Actor(actorManager, "block");
		ActorGraphicsComponent& blockGraphics = *new ActorGraphicsComponent(block, "block graphics");
		blockGraphics.mesh = box;
		blockGraphics.texture = texture;
		block.setTransformation(glm::translate(block.getTransformation(), position));
		new ActorPhysicsComponent(block, "Block Physics Component", 		new btBoxShape(btVector3(1, 1, 1)), 0, .45f, .35);
		blocks.push_back(&block);
		
		if (isRunning())
		{
			block.start();
		} //if
	} //createBlock
	
	void updateRect(float& minX, float& minY, float& maxX, float& maxY, const float& x, const float& y)
	{
		if (x < minX)
		{
			minX = x;
		} //if
		if (y < minY)
		{
			minY = y;
		} //if
		if (x > maxX)
		{
			maxX = x;
		} //if
		if (y > maxY)
		{
			maxY = y;
		} //if
	} //updateRect
	
	void loadLevel(istream& file)
	{
		if (inLevel)
		{
			return;
		} //if
	
		//these are used for knob placement
		float minX = 1000000;
		float minY = 1000000;
		float maxX = -1000000;
		float maxY = -1000000;
		
		//iterate over the file line by line and populate an x,y 2D grid with
		//cubes
		int x = 0;
		int y = 0;
		string line;
		while (file.good())
		{
			getline(file, line, '\n');
			
			x = 0;
			for (char& c : line)
			{
				switch (c)
				{
					//wall
					case 'w':
						createBlock(glm::vec3(x*scaling, 1*scaling, y*scaling), stone);
						createBlock(glm::vec3(x*scaling, 2*scaling, y*scaling), black);
						updateRect(minX, minY, maxX, maxY, x, y);
						break;
					//hole
					case ' ':
						break;
					//ground
					case 'g':
						createBlock(glm::vec3(x*scaling, 1*scaling, y*scaling), stone);
						updateRect(minX, minY, maxX, maxY, x, y);
						
						break;
					//start position
					case 's':
						createBlock(glm::vec3(x*scaling, 1*scaling, y*scaling), green);
						startPosition = glm::vec3(x, 1, y);
						updateRect(minX, minY, maxX, maxY, x, y);
						
						break;
					//goal position
					case 'f':
						createBlock(glm::vec3(x*scaling, 1*scaling, y*scaling), red);
						goal = glm::vec3(x*scaling, 2*scaling, y*scaling);
						updateRect(minX, minY, maxX, maxY, x, y);
						
						break;
					default:
						break;
				} //switch
				
				++x;
			} //for
			++y;
		} //while
		
		/*
		*	place player and knobs
		*/
		glm::mat4 transformation = bottomKnob->getTransformation();
		transformation[3][0] = (minX + maxX)/2*scaling;
		transformation[3][1] = 2*scaling;
		transformation[3][2] = (maxY + 1.5)*scaling;
		bottomKnob->setTransformation(transformation);
		
		transformation = rightKnob->getTransformation();
		transformation[3][0] = (maxX + 1.5)*scaling;
		transformation[3][1] = 2*scaling;
		transformation[3][2] = (minY + maxY)/2*scaling;
		rightKnob->setTransformation(transformation);
		
		initPlayer();
		
		graphics.worldOrigin[3][0] = (minX + maxX)/2*scaling;
		graphics.worldOrigin[3][2] = (minY + maxY)/2*scaling;
		
		setInLevel(true);
		currentTime = 0;
		
		if (win.getStatus() == sf::SoundSource::Status::Playing)
		{
			win.stop();
		} //if
		if (lose.getStatus() == sf::SoundSource::Status::Playing)
		{
			lose.stop();
		} //if
		if (gameLoop.getStatus() != sf::SoundSource::Status::Playing)
		{
			gameLoop.play();
		} //if
	} //loadLevel
	
	void initPlayer()
	{
		glm::mat4 transformation = player->getTransformation();
		transformation[3][0] = startPosition.x*scaling;
		transformation[3][1] = (startPosition.y + 1)*scaling;
		transformation[3][2] = startPosition.z*scaling;
		player->setTransformation(transformation);
	} //initPlayer
	
	void cleanLevel()
	{
		for (Actor* block : blocks)
		{
			delete block;
		} //for
		
		blocks.clear();
	} //cleanLevel()
	
	void resetBoardRotationState()
	{
		/*
		*	reset board rotation (knobs sync with board)
		*/
		graphics.worldRotationX = glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		graphics.worldRotationY = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		
		/*
		*	reset graphics rotation
		*/
		physics.setGravityDirection(btVector3(0, 1, 0));
	} //resetBoardState
	
	void updateScore()
	{
		stringstream ss;
		ss << totalScore;
		
		scoreDisplay->setText(ss.str());
	} //updateScore
	
	inline int calculateScore()
	{
		if (level == 0)
		{
			return 0;
		} //if
		
		return (int)(base/currentTime*multiplier*(level/10.0f));
	} //calculateScore
	
	void endLevel(const bool& won = true)
	{
		/*
		*	update score and reset state of world
		*/
		if (inLevel)
		{
			setInLevel(false);
		
			if (won)
			{
				totalScore = totalScore + calculateScore();
				updateScore();
			} //if
			
			resetBoardRotationState();
			
			graphics.worldOrigin[3][0] = 0;
			graphics.worldOrigin[3][1] = 0;
			graphics.worldOrigin[3][2] = 0;
			
			glm::mat4 t;
			rightKnob->setTransformation(t);
			bottomKnob->setTransformation(t);
			player->setTransformation(t);
			
			cleanLevel();
		} //if
	} //endLevel
	
	void nextLevel()
	{
		endLevel(true);
		++level;
		
		/*
		*	load file containing next level
		*/
		stringstream ss;
		ss << level;
		
		ifstream file;
		file.open(string("levels/level") + ss.str() + string(".txt"));
		
		//if level doesn't exist, end the game
		if (file.good())
		{
			updateLevel();
			loadLevel(file);
			file.close();
		} //if
		else
		{
			file.close();
			endGame(true);
		} //else
	} //nextLevel
	
	/*
	*	all of these updates should be sets
	*/
	void updateLives()
	{
		stringstream ss;
		ss << lives;
		
		lifeDisplay->setText(ss.str());
	} //updateLives
	
	void updateLevel()
	{
		stringstream ss;
		ss << level;
		
		levelDisplay->setText(ss.str());
	} //updateLevel
	
	void reduceLife()
	{
		--lives;
		updateLives();
		
		/*
		*	if the player is out of lives, end the game
		*/
		if (lives == 0)
		{
			endLevel(false);
			endGame(false);
			
			return;
		} //if
		
		/*
		*	if the player still has lives, reset the board state
		*/
		resetBoardRotationState();
		initPlayer();
	} //reduceLife
	
	void updateTimeDisplay()
	{
		int seconds = ((int)currentTime)%60;
		int minutes = ((int)currentTime)/60;
		
		stringstream ss;
		
		ss << minutes;
		string minutesStr(ss.str());
		ss.str("");
		
		ss << seconds;
		string secondsStr(ss.str());
		
		if (secondsStr.length() == 1)
		{
			secondsStr = "0" + secondsStr;
		} //if
		
		clockDisplay->setText(minutesStr + string(":") + secondsStr);
	} //updateTimeDisplay
	
	/*
	*	this is used to check if the ball has fallen into a hole or has
	*	made it to the end
	*/
	void updateLevelState(const long double& delta)
	{
		if (!paused && inLevel)
		{
			currentTime += delta;
			updateTimeDisplay();
		
			const glm::mat4& trans = player->getTransformation();
			pointLight->position.x = trans[3][0];
			pointLight->position.y = trans[3][1];
			pointLight->position.z = trans[3][2];
			
			glm::vec3 position(trans[3][0], trans[3][1], trans[3][2]);
			
			/*
			*	determine if the ball fell into a hole
			*/
			if (position.y < 0)
			{
				reduceLife();
				
				return;
			} //if
			
			/*
			*	determine if the player made it to the goal
			*/
			if (glm::distance(goal, position) <= 1)
			{
				nextLevel();
				
				return;
			} //if
			
			((ActorPhysicsComponent&)(player->components[typeid(PhysicsSystem)])).getBody().applyCentralImpulse(btVector3(0, -9.81*10, 0));
		} //if
	} //updateLevelState

	/*
	*	Engine Init and Destructions
	*/
	public:

		TestEngine(int argc, char** argv) :
			Engine(),
			window(sf::VideoMode::getDesktopMode(), "Labyrinth", sf::Style::Default, sf::ContextSettings(32, 0, 0, 3, 3)),
			gui0(window),
			gui1(window),
			gui2(window),
			actorManager(*new ActorManager(*this)),
			camera(*new Camera(actorManager)),
			graphics(*new GraphicsSystem(*this, window, gui0, gui1, gui2, camera)),
			input(*new InputSystem(*this, window, gui0, gui1, gui2)),
			physics(*new PhysicsSystem(*this)),
			vertexShader(GL_VERTEX_SHADER),
			fragmentShader(GL_FRAGMENT_SHADER)
		{
			/*
			*	settings
			*/
			startLives = 3;
			lives = startLives;
			level = 0;
			totalScore = 0;
			base = 1000;
			multiplier = 4.5;
			sensitivity = 1.25;
			lifeBonus = 25;
			scaling = 2.0f;
			paused = false;
			inEndMenu = false;
			inLevel = true;
			scoreboard = NULL;
			
			/*
			*	resources
			*/
			ball = NULL;
			box = NULL;
			knob = NULL;
			stone = NULL;
			lava = NULL;
			red = NULL;
			green = NULL;
			black = NULL;
		
			/*
			*	this is for FPS controls
			*/
			//window.setMouseCursorVisible(false);
		
			/*
			*	Setup global events (bad)
			*/
			input.event.tick.reg(&TestEngine::handleInput, this);
			event.tick.reg(&TestEngine::handleHeld, this);
			event.tick.reg(&TestEngine::updateLevelState, this);
			
			/*
			*	Setup GUIs
			*/
			gui1.setGlobalFont("fonts/DejaVuSans.ttf");

			loadWidgets(gui1, gui2);

			/*
			*	Setup shaders
			*/
			vertexShader.loadFromFile("shaders/shader.vert");
			vertexShader.compile();

			fragmentShader.loadFromFile("shaders/shader.frag");
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
			box = loadMesh("models/box.obj");
			knob = loadMesh("models/sphere.obj");
			ball = loadMesh("models/ball.obj");
			//ball = knob; //for testing purposes
			
			stone = new Texture("images/Tileable stone texture.jpg");
			lava = new Texture("images/lava.jpg");
			red = new Texture("images/16-red-watercolor-background-sml.jpg");
			green = new Texture("images/14-green-watercolor-background-sml.jpg");
			black = new Texture("images/seamlesstexture12_1200.jpg");
			
			ball->texture = lava;
			box->texture = stone;
			knob->texture = lava;

			/*
			*	Setup Actor Context
			*/
			actorManager.context.add(graphics);
			actorManager.context.add(input);
			actorManager.context.add(physics);

			/*
			*	Setup Actors
			*/
			player = new Actor(actorManager, "actor");
			bottomKnob = new Actor(actorManager, "bottomKnob");
			rightKnob = new Actor(actorManager, "rightKnob");

			/*
			*	setup components
			*/
			ActorGraphicsComponent& playerGraphics = *new ActorGraphicsComponent(*player, "Graphics Component");
			playerGraphics.mesh = ball;
			new ActorPhysicsComponent(*player, "Player Physics Component", new btSphereShape(1/1.1), 45, .65f, .15f);
			playerGraphics.mesh = ball;
			ActorGraphicsComponent& bottomKnobGraphics = *new ActorGraphicsComponent(*bottomKnob, "Graphics Component");
			bottomKnobGraphics.mesh = knob;
			bottomKnobGraphics.ignoreWorldRotation = true;
			ActorGraphicsComponent& rightKnobGraphics = *new ActorGraphicsComponent(*rightKnob, "Graphics Component");
			rightKnobGraphics.mesh = knob;
			rightKnobGraphics.ignoreWorldRotation = true;
			
			/*
			*	setup camera
			*/
			camera.rotate(glm::vec3(glm::pi<float>()/2, 0, 0));
			camera.setPosition(glm::vec3(0, -16, 0));
			new CameraInputControl(camera, "Camera Input Control", *this, window);
			
			/*
			*	setup lights
			*/
			
			ambientLight = new DirectionalLight();
			ambientLight->direction = glm::normalize(glm::vec3(0, 0, 1));
			ambientLight->color = glm::vec3(1, 1, 1);
			ambientLight->diffuseIntensity = 0;
			ambientLight->ambientIntensity = .4;
			graphics.lights.push_back(ambientLight);
			
			directionalLight = new DirectionalLight();
			directionalLight->direction = glm::normalize(glm::vec3(-1, 0, 0));
			directionalLight->color = glm::vec3(0, 1, 1);
			directionalLight->diffuseIntensity = .0001;
			directionalLight->ambientIntensity = 0;
			graphics.lights.push_back(directionalLight);
			
			pointLight = new PointLight();
			pointLight->position = glm::vec3(0, 2, 0);
			pointLight->color = glm::vec3(1, 0, 1);
			pointLight->diffuseIntensity = .2;
			pointLight->ambientIntensity = 0;
			pointLight->attenuation.constant = 1;
			pointLight->attenuation.linear = .4;
			pointLight->attenuation.exp = .2;
			graphics.lights.push_back(pointLight);
			
			/*
			SpotLight* spotLight = new SpotLight();
			spotLight->direction = glm::normalize(glm::vec3(0, -1, 0));
			spotLight->position = glm::vec3(0, 4, 0);
			spotLight->color = glm::vec3(0, .8, .8);
			spotLight->diffuseIntensity = 16;
			spotLight->ambientIntensity = 0;
			spotLight->attenuation.constant = 1;
			spotLight->attenuation.linear = .4;
			spotLight->attenuation.exp = .2;
			spotLight->cutoff = glm::pi<float>()/16;
			spotLights.push_back(spotLight);
			graphics.lights.push_back(spotLight);
			*/
			
			/*
			*	setup sounds
			*/
			gameLoop.openFromFile("sounds/World Vice.ogg");
			win.openFromFile("sounds/The Complex.ogg");
			lose.openFromFile("sounds/Ripples.ogg");
			
			gameLoop.setLoop(true);
			win.setLoop(true);
			lose.setLoop(true);
			
			updateLives();
			updateScore();
			setInLevel(false);
			
			/*
			*	load first level
			*/
			nextLevel();
		} //TestEngine

		virtual ~TestEngine()
		{
			//models
			if (ball && ball != knob)
			{
				delete ball;
			} //if
			if (box)
			{
				delete box;
			} //if
			if (knob)
			{
				delete knob;
			} //if
			
			//textures
			if (stone)
			{
				delete stone;
			} //if
			if (lava)
			{
				delete lava;
			} //if
			if (red)
			{
				delete red;
			} //if
			if (green)
			{
				delete green;
			} //if
			if (black)
			{
				delete black;
			} //if
			
			if (gameLoop.getStatus() == sf::SoundSource::Status::Playing)
			{
				gameLoop.stop();
			} //if
			if (win.getStatus() == sf::SoundSource::Status::Playing)
			{
				win.stop();
			} //if
			if (lose.getStatus() == sf::SoundSource::Status::Playing)
			{
				lose.stop();
			} //if
			
			//lights
			for (Light* light : graphics.lights)
			{
				delete light;
			} //for
		} //~TestEngine
}; //TestEngine
