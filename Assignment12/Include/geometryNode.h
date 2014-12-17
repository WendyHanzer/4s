#pragma once

#include "sceneNode.h"
#include "mesh.h"

class SceneNodePosition;
class SceneNode;

class GeometryData : public SceneNodePosition
{
public:
	Mesh* mesh;

	void copyTo(GeometryData& position)
	{
		SceneNodePosition::copyTo(position);

		position.mesh = mesh;
	} //copyTo
}; //GeometryData

class SceneNode;

class GeometryNode : public SceneNode
{
public:
	Mesh* mesh;

	virtual void onVisit(SceneFrame& frame, glm::mat4& position)
	{
		static GeometryData geometry;

		copyTo(geometry);

		frame.geometry.push(geometry);
	}

	void copyTo(GeometryData& position)
	{
		SceneNode::copyTo(position);

		position.mesh = mesh;
	} //copyTo
}; //GeometryNode