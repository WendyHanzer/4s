#pragma once

#include "indexedArray.h"

class DirectionalLight;
class AmbientLight;
class PointLightData;
class SpotLightData;
class GeometryData;

struct SceneFrame
{
	IndexedArray<DirectionalLight> directionalLight;
	IndexedArray<AmbientLight> ambientLight;
	IndexedArray<PointLightData> pointLight;
	IndexedArray<SpotLightData> spotLight;
	IndexedArray<GeometryData> geometry;

	void clear()
	{
		directionalLight.clear();
		ambientLight.clear();
		pointLight.clear();
		spotLight.clear();
		geometry.clear();
	} //claer
}; //SceneFrame