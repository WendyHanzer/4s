#pragma once

#include "AspectType.h"

class Aspect
{
	private:

		const AspectType* aspectType;

	protected:

		Aspect(const AspectType* aspectType) : aspectType(aspectType) { }

	public:

		inline AspectType* getAspectType() { return aspectType; }
}; //Aspect