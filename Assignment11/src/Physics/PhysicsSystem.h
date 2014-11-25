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
		
		float gravity;
		btVector3 rotation;

	protected:
	
		virtual void onTick(const long double& delta);
		
	public:

		PhysicsSystem(Engine& engine, const float& gravity = -9.81, const btVector3& rotation = btVector3(0, 1, 0));
		virtual ~PhysicsSystem();
		
		inline btDiscreteDynamicsWorld& getDynamicWorld() { return dynamicsWorld; }
		
		inline const btVector3& getGravityDirection() { return rotation; }
		inline const float& getGravity() { return gravity; }
		
		inline void setGravityDirection(const btVector3& rotation)
		{
			this->rotation = this->rotation + rotation;
			this->rotation.safeNormalize();
			
			dynamicsWorld.setGravity(gravity*rotation);
		} //setGravityDirection
		
		inline void setGravity(const float& gravity)
		{
			this->gravity = gravity;
			
			dynamicsWorld.setGravity(gravity*rotation);
		} //setGravity

}; //PhysicsSystem
