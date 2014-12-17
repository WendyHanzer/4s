#pragma once

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>

#include "sceneFrame.h"

struct GraphicsFrame
{
	SceneFrame sceneFrame;
	glm::mat4 V;
	glm::mat4 P;
	glm::mat4 VP;

	int x;
	int y;
	int width;
	int height;

	void clear()
	{
		sceneFrame.clear();
	} //clear
}; //GraphicsFrame