#pragma once

#include <btBulletDynamicsCommon.h>

#include "../Engine/System/System.h"

class PhysicsSystem : public System
{
	private:
	
		btDefaultCollisionConfiguration& collisionConfiguration;
		btCollisionDispatcher& dispatcher;
		btBroadphaseInterface& broadphase;
		btSequentialImpulseConstraintSolver& solver;
		btDiscreteDynamicsWorld& dynamicsWorld;

	protected:
	
		virtual void onTick(const long double& delta);
		
	public:

		PhysicsSystem(Engine& engine, const btVector3& gravity = btVector3(0, -9.81, 0));
		virtual ~PhysicsSystem();
		
		inline btDiscreteDynamicsWorld& getDynamicWorld() { return dynamicsWorld; }

}; //PhysicsSystem
