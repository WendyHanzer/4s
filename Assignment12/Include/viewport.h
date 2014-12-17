#pragma once

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>

#include <iostream>

#include <GL/glew.h>

#include "graphicsFrame.h"
#include "cameraNode.h"
#include "geometryNode.h"
#include "pointLight.h"

//due to lack of time, a generic buffer class was created and put into the viewport
//originally it was going to be very dynamic, adding passes freely

//supported by ogldev


//threw in general rendering too...

class Viewport
{
protected:
	int currentWidth;
	int currentHeight;
	int currentX;
	int currentY;

public:
	CameraNode* camera;

	int width;
	int height;
	int x;
	int y;
	float fieldOfView;

	virtual int getX() { return x; }
	virtual int getY() { return y; }
	virtual int getWidth() { return width; }
	virtual int getHeight() { return height; }

	Viewport()
	{
		width = 0;
		height = 0;
		x = 0;
		y = 0;
		currentWidth = -1;
		currentHeight = -1;
		currentX = -1;
		currentY = -1;
		fieldOfView = 0;
		fieldOfView = 45.0f;
		camera = NULL;
	} //Viewport

	virtual ~Viewport()
	{
	} //~Viewport

	inline glm::mat4 getP()
	{
		if (camera == NULL) { return glm::mat4(); }
		return glm::perspective(fieldOfView*camera->zoom, ((float)getWidth()) / getHeight(), camera->nearZ, camera->farZ);
	} //getProjection

	inline glm::mat4 getV()
	{
		if (camera == NULL) { return glm::mat4(); }
	} //getV

	inline glm::mat4 getVP()
	{
		return getP()*getV();
	} //getVP

	void activate()
	{
		glViewport(getX(), getY(), getWidth(), getHeight());
	} //activate

	virtual void render(GraphicsFrame& frame)
	{
	} //render
}; //Viewport