#pragma once

#include "lightNode.h"

class PointLightData : public LightNodeData
{
public:
	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponential;

	void copyTo(PointLightData& light)
	{
		LightNodeData::copyTo(light);

		light.attenuationConstant = attenuationConstant;
		light.attenuationExponential = attenuationExponential;
		light.attenuationLinear = attenuationLinear;
	} //copyTo
}; //LightNodeData

class PointLight : public LightNode
{
public:
	struct
	{
		float constant;
		float linear;
		float exponential;
	} attenuation;

protected:
	virtual void onVisit(SceneFrame& frame, glm::mat4& position)
	{
		static PointLightData data;

		copyTo(data);

		frame.pointLight.push(data);
	} //onVisit

	void copyTo(PointLightData& position)
	{
		LightNode::copyTo(position);

		position.attenuationConstant = attenuation.constant;
		position.attenuationLinear = attenuation.linear;
		position.attenuationExponential = attenuation.exponential;
	} //copyTo
}; //LightNode