#pragma once

#define GLM_FORCE_RADIANS

#include <string>
#include <btBulletCollisionCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../Graphics/mesh.h"
#include "../../../Engine/Entity/Component/Component.h"
#include "../../../Physics/PhysicsSystem.h"
#include "ActorGraphicsComponent.h"
#include "../Actor.h"
#include "../../../Utils/PhysicsUtils.h"

/*
// Define collision enumeration for objects
#ifndef BIT
#define BIT(x) (1<<(x))
enum collisiontypes {
    COL_NOTHING	= 0, 		//<Collide with nothing>
    COL_WALLS	= BIT(0), 	//<Collide with walls>
    COL_MIDWALL	= BIT(1), 	//<Collide with middle wall>
	COL_PUCK 	= BIT(2),	//<Collide with puck>
	COL_PADDLES = BIT(3), 	//<Collide with paddles>
	COL_TABLE	= BIT(4),	//<Collide with table>
	COL_GOALS 	= BIT(5)	//<Collide with goal>
};
#endif
*/

class ActorPhysicsComponent : public Component
{
	private:

		btRigidBody* body;

		void onTransformationChange()
		{
			glm::mat4 glTransformation = ((const Actor&)getController()).getTransformation();
			btTransform transformation = convertGLMTransformToBullet(glTransformation);
			setTransformation(transformation);
		} //setMVPofBulletObj
	protected:
		Event<void>::id actionTransformationChange;

		virtual void onChangeController()
		{
		} //onChangeController

		//add body
		virtual void onStart()
		{
			((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, group, mask);

			/*
			if( bodyName->compare( "table" ) == 0 )
			{
				collidesWith = COL_NOTHING;
			}
			else if( bodyName->compare( "puck" ) == 0 )
			{
				collidesWith = COL_PADDLES | COL_WALLS | COL_TABLE;
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, COL_PUCK, collidesWith);
				std::cout << "Found puck" << std::endl;
			}
			else if( bodyName->compare( "paddle" ) == 0 )
			{
				collidesWith = COL_PUCK | COL_WALLS | COL_MIDWALL | COL_TABLE;
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, COL_PADDLES, collidesWith);
				std::cout << "Found paddle" << std::endl;
			}
			else if( bodyName->compare( "goal" ) == 0 )
			{
				collidesWith = COL_NOTHING;
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, COL_GOALS, collidesWith);
				std::cout << "Found goal" << std::endl;
			}
			else if( bodyName->compare( "wall" ) == 0 )
			{
				collidesWith = COL_PUCK | COL_PADDLES;
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, COL_WALLS, collidesWith);
				std::cout << "Found wall" << std::endl;
			}
			else if( bodyName->compare( "MiddleWall" ) == 0 )
			{
				collidesWith = COL_PADDLES;
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().addRigidBody(body, COL_MIDWALL, collidesWith);
				std::cout << "Found middle wall" << std::endl;
			}
			else
			{
				std::cout << "Found something else" << std::endl;
			}
			*/

			Component::onStart();
		} //onStart

		virtual void onTick(const long double& delta)
		{
			actionTransformationChange->enable = false;
			btTransform worldTrans;
			getTransformation(worldTrans);
			((Actor&)getController()).setTransformation(convertBulletTransformToGLM(worldTrans));
			actionTransformationChange->enable = true;
			Component::onTick(delta);
		} //onTick

		inline btMotionState& getMotionState() 	{ return *body->getMotionState(); }
		inline void getTransformation(btTransform& worldTrans) { getMotionState().getWorldTransform(worldTrans); }
		inline void setTransformation(btTransform& worldTrans) { getMotionState().setWorldTransform(worldTrans); }

	public:
	
		const float mass;
		const short group;
		const short mask;

		inline btRigidBody& getBody() 					{ return *body; }
		inline btCollisionShape& getShape() 			{ return *((btCollisionShape*)body->getCollisionShape()); }
		inline btStridingMeshInterface& getMesh() 		{ return *((btStridingMeshInterface*)((btConvexTriangleMeshShape&)getShape()).getMeshInterface()); }

		ActorPhysicsComponent(
			/*
			*	arguments
			*/
			Actor& controller, const std::string name, btCollisionShape* shape = NULL,
			//mass						  //elasticity					   //friction
			const btScalar& mass = 0.0f,  const float& restitution = 0.0f, const float& friction = 0.0f,
			//group				//groups it collides with
			const short& group = -1, const short& mask = -1)

			/*
			*	init
			*/
			:
			Component(controller, controller.getManager().context[typeid(PhysicsSystem)], typeid(PhysicsSystem), name),
			mass(mass),
			group(group),
			mask(mask)
		{
			if (!shape)
			{
				Mesh* glMesh = ((ActorGraphicsComponent&)getController().components[typeid(GraphicsSystem)]).mesh;

				if (!glMesh)
				{
					//uh oh!

					throw 1;
				} //if

				btTriangleIndexVertexArray* mesh = new btTriangleIndexVertexArray(
						glMesh->indices.size()/3,
						glMesh->indices.data(),
						sizeof(GLint)*3,
						glMesh->vertices.size()/3,
						glMesh->vertices.data(),
						sizeof(GLfloat)*3);
						
				shape = new btConvexTriangleMeshShape(mesh, true);
			} //if

			const glm::mat4& glTransformation = ((Actor&)getController()).getTransformation();
			btTransform transformation = convertGLMTransformToBullet(glTransformation);
			shape->setLocalScaling(convertGLMVectorToBullet(getScalingGLM(glTransformation)));
			btVector3 localInertia(0, 0, 0);
			shape->calculateLocalInertia(mass, localInertia);

			btDefaultMotionState* motionState = new btDefaultMotionState(transformation);
			body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, motionState, shape, localInertia));

			body->setRestitution(restitution);
			body->setFriction(friction);
			body->setCenterOfMassTransform(transformation);

			body->forceActivationState(DISABLE_DEACTIVATION);

			actionTransformationChange = ((Actor&)getController()).eventTransformationChange.reg(&ActorPhysicsComponent::onTransformationChange, this);

		} //ActorPhysicsComponent

		//remove body
		virtual void onStop()
		{
			Component::onStop();

			((PhysicsSystem&)(System&)mapping).getDynamicWorld().removeRigidBody(body);
		} //onStop

		virtual ~ActorPhysicsComponent()
		{
			destroy();

			if (isRunning())
			{
				((PhysicsSystem&)(System&)mapping).getDynamicWorld().removeRigidBody(body);
			} //if

			actionTransformationChange->unreg();

			if (typeid(getShape()) == typeid(btConvexTriangleMeshShape))
			{
				delete &getMesh();
			} //if

			delete &getShape();
			delete &getMotionState();
			delete &getBody();
		} //~ActorPhysicsComponent

}; //ActorPhysicsComponent

