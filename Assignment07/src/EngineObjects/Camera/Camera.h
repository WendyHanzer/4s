#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

class Camera : public MoveableObject
{
	private:

	public:
		glm::mat4 projection;
		/*
		*	Constructors
		*/
		Camera();
		~Camera();

}; //Camera