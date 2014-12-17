#pragma once

#include "sceneNode.h"
#include "directionalLight.h"

class LightNodeData : public DirectionalLight, public SceneNodePosition
{
public:
	void copyTo(LightNodeData& light)
	{
		AmbientLight::copyTo(light);
		SceneNodePosition::copyTo(light);
	} //copyTo
}; //LightNodeData

class LightNode : public SceneNode, public DirectionalLight
{
protected:
	void copyTo(LightNodeData& position)
	{
		SceneNode::copyTo(position);
		DirectionalLight::copyTo(position);
	} //copyTo
}; //LightNode