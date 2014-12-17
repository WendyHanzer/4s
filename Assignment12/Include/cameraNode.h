#pragma once

#include "sceneNode.h"

struct CameraNode : public SceneNode
{
	//projection
	float nearZ;
	float farZ;
	//ratio - retrieved from viewport
	float zoom;

	//camera
	//worldPosition

	CameraNode() { zoom = 1.0f; nearZ = .01f; farZ = 2000.0f; }

	inline const glm::mat4& getView()
	{
		return worldPosition;
	} //getProjection
}; //CameraNode