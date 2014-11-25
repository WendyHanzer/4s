#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(Engine& engine, const float& gravity, const btVector3& rotation) :
	System(engine),
	collisionConfiguration(*new btDefaultCollisionConfiguration()),
	dispatcher(*new btCollisionDispatcher(&collisionConfiguration)),
	broadphase(*new btDbvtBroadphase()),
	solver(*new btSequentialImpulseConstraintSolver),
	dynamicsWorld(*new btDiscreteDynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration)),
	gravity(gravity),
	rotation(rotation.normalized())
{
    dynamicsWorld.setGravity(gravity*rotation);
} //PhysicsSystem

PhysicsSystem::~PhysicsSystem()
{
	destroy();

    delete &dynamicsWorld;
    delete &solver;
    delete &dispatcher;
    delete &broadphase;
    delete &collisionConfiguration;
} //~PhysicsSystem

void PhysicsSystem::onTick(const long double& delta)
{
	dynamicsWorld.stepSimulation(delta, 10, 1.0f/60.0f);

	System::onTick(delta);
} //onTick
