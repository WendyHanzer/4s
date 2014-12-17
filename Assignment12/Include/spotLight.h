#pragma once

#include "pointLight.h"

class PointLightData;

class SpotLightData : public PointLightData
{
public:
	float cutoff;

	void copyTo(SpotLightData& light)
	{
		PointLightData::copyTo(light);

		light.cutoff = cutoff;
	} //copyTo
}; //LightNodeData

class SpotLight : public PointLight
{
public:
	float cutoff;

protected:
	virtual void onVisit(SceneFrame& frame, glm::mat4& position)
	{
		static SpotLightData data;

		copyTo(data);

		frame.spotLight.push(data);
	} //onVisit

	void copyTo(SpotLightData& position)
	{
		PointLight::copyTo(position);

		position.cutoff = cutoff;
	} //copyTo
}; //LightNode