#pragma once

#include "../Computer.h"

class Computer;

class ComputerAI
{
	public:
	
		virtual void onTick(const long double& delta, Computer& computer);
		virtual void init(Computer& computer);
		
		ComputerAI() { }
		virtual ~ComputerAI() { }
		
}; //ComputerAI
