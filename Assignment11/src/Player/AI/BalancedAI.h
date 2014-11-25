#pragma once

#include <random>

#include "ComputerAI.h"

class BalancedAI : public ComputerAI
{
	bool onOurSide;
	bool wasOnOurSide;
	long double timeout;
	
	random_device rand;
	
	void updateSide(Computer& computer)
	{
		//middle = 0, 0, -6
	
		if (computer.goal.getTransformation()[3][2] < 0)
		{
			if (computer.puck.getTransformation()[3][2] < - 6)
			{
				onOurSide = true;
			} //if
		} //if
		else if (computer.puck.getTransformation()[3][2] > - 6)
		{
			onOurSide = true;
		} //if
		else
		{
			onOurSide = false;
		} //else
	} //updateSide
	
	glm::vec4 getSide(Computer& computer)
	{
		if (computer.goal.getTransformation()[3][2] < 0)
		{
			return glm::vec4(-2.5, -15, 2.5, -5);
		} //if
		else
		{
			return glm::vec4(-2.5, -7, 2.5, 3.8);
		} //if
	} //getSide
	
	long double getRand(float min, float max)
	{
		long double r = rand();
		
		r -= rand.min();
		r /= (rand.max() - rand.min());
		
		return r*(max - min) + min;
	} //getRand

	public:
		
		virtual void init(Computer& computer)
		{
			updateSide(computer);
			
			wasOnOurSide = onOurSide;
			
			timeout = 0;
		} //init
	
		virtual void onTick(const long double& delta, Computer& computer)
		{
			//middle = 0, 0, -6
			updateSide(computer);
			
			if (onOurSide)
			{
				if (wasOnOurSide)
				{
					//attack
					new ActorCommandFollow(computer.paddle, "follow", computer.puck, -1, -40);
					
					//should calculate how to hit the puck in order to make it go into the goal, but too
					//much work
				} //if
				else
				{
					//defend
					new ActorCommandFollow(computer.paddle, "follow", computer.puck, -1, -10);
				} //else
			} //if
			else
			{
				//wander
				timeout += delta;
				
				if (timeout == 1.3)
				{
					const glm::vec4 dim(getSide(computer));
					
					new ActorCommandMove(computer.paddle, "move", glm::vec3(getRand(dim.x, dim.z), 0, getRand(dim.y, dim.w)), -1, .3);
					
					timeout = 0;
				} //if
			} //else
		} //onTick
		
		BalancedAI() : ComputerAI(), onOurSide(false), wasOnOurSide(false), timeout(0)
		{
		} //BalancedAI
		
		//virtual ~BalancedAI() { }
		
}; //BalancedAI
