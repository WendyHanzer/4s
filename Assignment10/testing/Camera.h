#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
// Mat4 to Quat
glm::vec3 lookAtPt = direction;
glm::mat4 rotMatrix = glm::lookAt(glm::vec3(0), lookAtPt, up);
glm::quat rotation = glm::quat_cast(rotMatrix);

// Quat to Mat4
glm::mat4 identityMat = glm::mat4(1.0f);
glm::mat4 rotMatrix = glm::mat4_cast(rotation);   //rotation is glm::quat
glm::mat4 transMatrix = glm::translate(identityMat, position);
glm::mat4 viewMatrix = rotMatrix * glm::inverse(transMatrix);

inline glm::vec3 offset2Vector(const float& distance, const glm::quat& orientation)
{
	return orientation*(distance*glm::vec3(0, 0, -1));
} //offset2Vector
*/

class Camera
{
	private:
	
		static const glm::vec3 up;
		glm::mat4& target;
		glm::mat4 view;
		
	public:
	
		inline const glm::mat4& getView() { return view; }
		inline const glm::mat4& getTarget() { return target; }
		
		inline glm::quat getRotation() { return glm::quat_cast(view); }
		inline const glm::vec3& getPosition() { return view[3]; }
		inline const float getZoom() { return glm::distance(view[3], position[3]); }
		
		inline void setTarget(glm::mat4& target)
		{
			const float zoom = getZoom();
			const glm::quat rotation = getRotation();
			
		
			if ((glm::vec3)view[3] == (glm::vec3)target[3])
			{
			} //if
			else
			{
				view = glm::lookAt(view[3], target[3], up);
			} //else
		} //setTarget
		
		inline void update()
		{
		} //update
}; //Camera
