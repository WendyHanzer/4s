#pragma once

#include "graphicsFrame.h"

class RenderPass
{
public:
	virtual void render(GraphicsFrame* frame);
}; //RenderPass