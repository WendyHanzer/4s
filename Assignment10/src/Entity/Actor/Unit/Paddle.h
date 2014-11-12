#pragma once

#include "../Actor.h"
#include "../../../Graphics/mesh.h"
#include "../../../Graphics/GraphicsSystem.h"
#include "../../../Physics/PhysicsSystem.h"
#include "../Component/ActorGraphicsComponent.h"
#include "../Component/ActorPhysicsComponent.h"

class Paddle : public Actor
{
	public:
	
		enum SHAPE
		{
			SHAPE_INVALID = 0,
			SHAPE_BOX,
			SHAPE_CYLINDER
		}; //SHAPE
	
	private:
		
		SHAPE shape;

	public:
	
		Paddle(ActorManager& actorManager, const std::string name) :
		Actor(actorManager, name)
		{
		} //Paddle
		
		//create graphics component
		//set graphics component program
		//create physics component with settings
		//set shape
		//*would be better with a mesh manager*
		void setShape(const SHAPE& shape, Mesh& mesh)
		{
			if (this->shape == shape)
			{
				return;
			} //if
			
			this->shape = shape;
			
			((ActorGraphicsComponent&)components[typeid(GraphicsSystem)]).mesh = &mesh;
			
			ActorPhysicsComponent* physicsComponent = &(ActorPhysicsComponent&)components[typeid(PhysicsSystem)];
			btRigidBody& body = physicsComponent->getBody();
			
			const short group = physicsComponent->group;
			const short mask = physicsComponent->mask;
			const float mass = physicsComponent->mass;
			
			const float restitution = body.getRestitution();
			const float friction = body.getFriction();
			
			const btVector3 linearVelocity = body.getLinearVelocity();
			
			const std::string name = physicsComponent->name;
			
			delete physicsComponent;
			
			/*
			*	no time to fix mesh for triangle mesh
			*/
			switch (shape)
			{
				case SHAPE_BOX:
					physicsComponent = new ActorPhysicsComponent(*this, name, new btBoxShape(btVector3(1, 1, 1)), mass, restitution, friction, group, mask);
					break;
				case SHAPE_CYLINDER:
					physicsComponent = new ActorPhysicsComponent(*this, name, new btCylinderShape(btVector3(.4, .4, .4)), mass, restitution, friction, group, mask);
					break;
				default:
					break;
			} //switch
			
			physicsComponent->getBody().setLinearVelocity(linearVelocity);
		} //setShape
}; //Paddle
