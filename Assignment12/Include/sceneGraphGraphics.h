#pragma once

#include <vector>

#include "directionalLight.h"
#include "ambientLight.h"
#include "graphicsFrame.h"
#include "viewport.h"
#include "sceneGraph.h"
#include "renderer.h"

class SceneGraphGraphics
{
public:
	std::vector <Viewport*> viewport;

	SceneGraph graph;

	GraphicsFrame frame;

	void buildFrame()
	{
		//take a snapsnot of scene
		//currently not multi-threaded
		graph.root.visit(frame.sceneFrame);

		//capture all lights
		for (DirectionalLight* light : graph.directionalLight)
		{
			frame.sceneFrame.directionalLight.push(*light);
		} //for

		//capture all ambient lights
		for (AmbientLight* light : graph.ambientLight)
		{
			frame.sceneFrame.ambientLight.push(*light);
		} //for
	} //buildFrame

	void render()
	{
		if (!viewport.empty())
		{
			buildFrame();

			for (Viewport* v : viewport)
			{
				frame.P = v->getP();
				frame.V = v->getV();
				frame.VP = frame.P*frame.V;
				frame.x = v->getX();
				frame.y = v->getY();
				frame.height = v->getHeight();
				frame.width = v->getWidth();

				v->render(frame);
			} //for

			frame.clear();
		} //ifr
	} //render
}; //SceneGraphGraphics