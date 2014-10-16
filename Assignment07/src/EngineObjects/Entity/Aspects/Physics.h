#pragma once

#include "../Entity.h"
#include "../../Scene/SceneNode.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion>

class Physics
{ /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
		Entity* entity;

		glm::quat angularVelocity;
		glm::quat angularAcceleration;

		glm::vec3 velocity;
		glm::vec3 acceleration;

		/*
		*	be able to handle
		*
		*		velocity
		*		speedDirection
		*		speed
		*
		*		acceleration
		*		accelerationDirection
		*		accelerationScalar
		*/

		/*
		//quaternion to unit vector
		//								normal
		glm::vec3 direction = quat.inverse()*glm::vec3(0,1,0); // or -1 depending on direction

*/

		/*
		Matrix4    m_worldMatrix;
		Quaternion m_rotation;
		Vector3    m_scale;
		Vector3    m_translation;
		*/

		/*
		glm::mat4 rot = glm::toMat4(orientation);
			//orientation = glm::quat
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 final = rot * trans;
		*/

		/*
		void RotateBy(float radians, Vector3 rotation_axis)
		{
		    GL.MatrixMode(ModelView);

		    m_rotation = Quaternion.FromAxisAngle(rotation_axis, radians) * m_rotation;

		    m_worldMatrix = Matrix4.Scale(m_scale) *
		                    Matrix4.Rotate(m_rotation) *
		                    Matrix4.CreateTranslation(m_translation);

		    GL.MultMatrix(ref m_worldMatrix);
		}
		*/

		void onTick(double delta); //private, runs whenever entity ticks

		void onDestroy(Entity* entity);
		//	runs when entity is destroyed
/*
*****************************************************************
*
*/	public: /*
*
*
*		Fields
*		----------------------------------
*/


/*		Constructors
*		----------------------------
*/
			Physics(Entity* entity);
			//	register to entity.eventTick
			//	register to entity.eventDestroy

			~Physics();
			//	removes aspect from entity, can either be called directly
			//	or when entity is destroyed

/*
*		Movements
*		----------------------------------
*/
		/*
		*	Position
		*/
			void setVelocity(glm::vec3 velocity);
			void addVelocity(glm::vec3 velocity);
			glm::vec3 getVelocity();

			void setAcceleration(glm::vec3 acceleration);
			void addAcceleration(glm::vec3 acceleration);
			glm::vec3 getAcceleration();

			void setSpeed(float speed);
			void addSpeed(float speed);
			float getSpeed();

			void setAcceleration(float acceleration);
			void addAcceleration(float acceleration);
			float getAcceleration();

		/*
		*	Direction
		*/
			void setAngularVelocity(glm::vec3 angularVelocity);
			void setAngularVelocity(glm::quat angularVelocity);
			void addAngularVelocity(float radians, glm::vec3 axis);
			glm::quat getAngularVelocity();

			void setAngularAcceleration(glm::vec3 angularAcceleration);
			void setAngularAcceleration(glm::quat angularAcceleration);
			void addAngularAcceleration(float radians, glm::vec3 axis);
			glm::quat getAngularAcceleration();

//	etc






/*
*****************************************************************/

}; //Physics
