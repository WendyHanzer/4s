#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

#include "../AI/AISystem.h"
#include "../Entity/Actor/Doodad/Goal.h"
#include "../Entity/Actor/Unit/Paddle.h"
#include "../Entity/Actor/Component/AI/Command/ActorCommandStop.h"
#include "Score.h"

class Player : public DestructBehavior<>
{
	private:
		Event<void>::id actionScore;
		Event<void>::id actionReset;
		Event<void>::id actionOpponentDestroy;
	
		Player* opponent;
	
		void resetPosition(const bool& hasPuck)
		{
			glm::mat4 trans = paddle.getTransformation();
			
			trans[3][0] = paddlePosition.x;
			trans[3][1] = paddlePosition.y;
			trans[3][2] = paddlePosition.z;
			
			paddle.setTransformation(trans);
			
			new ActorCommandStop(paddle);
			
			if (hasPuck)
			{
				trans = puck.getTransformation();
		
				trans[3][0] = puckPosition.x;
				trans[3][1] = puckPosition.y;
				trans[3][2] = puckPosition.z;
		
				puck.setTransformation(trans);
			} //if
		} //onPuckReceive
		
		void onScore()
		{
			score.setScore(score.getScore() + 1);
			
			resetPosition(false);
		} //onScore
		
		void clearOpponent()
		{
			if (opponent)
			{
				actionScore->unreg();
				actionOpponentDestroy->unreg();
			} //if
			
			opponent = NULL;
		} //clearOpponent
		
	protected:
	
		Player(const std::string& name, Score& score, Paddle& paddle, Goal& goal, Actor& puck, Engine& engine) :
			DestructBehavior(engine.event),
			opponent(NULL),
			name(name),
			score(score),
			paddle(paddle),
			goal(goal),
			puck(puck)
		{
			if (!paddle.components.hasComponent(typeid(AISystem)))
			{
				new ActorAIComponent(paddle, "ai");
			} //if
			
			actionReset = goal.receive.reg(&Player::resetPosition, this, true);
		} //Player

	public:
	
		const std::string name;
		
		Score& score;
		Paddle& paddle;
		Goal& goal;
		Actor& puck;
		
		const glm::vec3 paddlePosition;
		const glm::vec3 puckPosition;
		
		void reset(const bool& hasPuck)
		{
			resetPosition(hasPuck);
			
			score.setScore(0);
		} //reset
		
		virtual ~Player()
		{
			if (opponent)
			{
				actionScore->unreg();
				actionOpponentDestroy->unreg();
			} //if
			
			actionReset->unreg();
		
			destroy();
		} //~Player
		
		void setOpponent(Player* player)
		{
			clearOpponent();
			
			if (player)
			{
				opponent = player;
			
				actionScore = opponent->goal.receive.reg(&Player::onScore, this);
				actionOpponentDestroy = opponent->event.destroy.reg(&Player::clearOpponent, this);
			} //if
		} //setOpponent
		
		Player* getOpponent()
		{
			return opponent;
		} //getOpponent
}; //Player
