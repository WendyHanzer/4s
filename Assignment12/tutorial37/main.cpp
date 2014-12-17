#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "engine_common.h"
#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ds_geom_pass_tech.h"
#include "ds_point_light_pass_tech.h"
#include "ds_dir_light_pass_tech.h"
#include "null_technique.h"
#include "ogldev_glut_backend.h"
#include "mesh.h"

#include "sceneNode.h"
#include "cameraNode.h"
#include "sceneGraph.h"
#include "sceneGraphGraphics.h"
#include "demoViewport.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "geometryNode.h"
#include "spotLight.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "ambientLight.h"


#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

class Tutorial37 : public ICallbacks, public OgldevApp
{
public:

	SceneGraphGraphics scene1;

	DemoViewport viewport1;
	DemoViewport viewport2;

	CameraNode camera1;
	CameraNode camera2;

	DSGeomPassTech m_DSGeomPassTech;
	DSPointLightPassTech m_DSPointLightPassTech;
	DSDirLightPassTech m_DSDirLightPassTech;
	NullTechnique m_nullTech;


	float scale;
	SpotLight spotLight[3];
	DirectionalLight dirLight[3];
	PointLight pointLight[3];
	AmbientLight ambientLight[3];


	Mesh box;
	Mesh sphere;
	Mesh quad;
	Mesh cone;

	GeometryNode boxNode[5];

	Tutorial37()
	{
		scale = 0.0f;

		InitLights();
		InitBoxPositions();
	}

	bool Init()
	{
		viewport1.fieldOfView = 45.0f;
		viewport1.width = WINDOW_WIDTH / 2;
		viewport1.height = WINDOW_HEIGHT;
		viewport1.camera = &camera1;

		viewport2.fieldOfView = 45.0f;
		viewport2.width = WINDOW_WIDTH / 2;
		viewport2.height = WINDOW_HEIGHT;
		viewport2.x = WINDOW_WIDTH / 2;
		viewport2.camera = &camera2;

		camera1.nearZ = .1f;
		camera1.farZ = 1000.0f;
		camera1.zoom = 1;

		camera2.nearZ = .1f;
		camera2.farZ = 1000.0f;
		camera2.zoom = 1;

		scene1.viewport.push_back(&viewport1);
		scene1.viewport.push_back(&viewport2);

		scene1.graph.root.children.push_back(&boxNode[0]);
		scene1.graph.root.children.push_back(&boxNode[1]);
		scene1.graph.root.children.push_back(&boxNode[2]);
		scene1.graph.root.children.push_back(&boxNode[3]);
		scene1.graph.root.children.push_back(&boxNode[4]);
		scene1.graph.root.children.push_back(&pointLight[0]);
		scene1.graph.root.children.push_back(&pointLight[1]);
		scene1.graph.root.children.push_back(&pointLight[2]);
		scene1.graph.root.children.push_back(&camera1);
		scene1.graph.root.children.push_back(&camera2);

		scene1.graph.directionalLight.push_back(&dirLight[0]);

		camera1.position = glm::vec3(0, 0, 15);
		camera1.setRotation(glm::vec3(0, 0, -1));

		camera2.position = glm::vec3(0, 0, 0);
		//camera2.setRotation(glm::vec3(0, 0, -10));

		if (!m_DSGeomPassTech.Init()) {
			printf("Error initializing DSGeomPassTech\n");
			return false;
		}

		m_DSGeomPassTech.Enable();
		m_DSGeomPassTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

		if (!m_DSPointLightPassTech.Init()) {
			printf("Error initializing DSPointLightPassTech\n");
			return false;
		}

		m_DSPointLightPassTech.Enable();

		m_DSPointLightPassTech.SetPositionTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_POSITION);
		m_DSPointLightPassTech.SetColorTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_DIFFUSE);
		m_DSPointLightPassTech.SetNormalTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_NORMAL);
		m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

		if (!m_DSDirLightPassTech.Init()) {
			printf("Error initializing DSDirLightPassTech\n");
			return false;
		}

		m_DSDirLightPassTech.Enable();

		m_DSDirLightPassTech.SetPositionTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_POSITION);
		m_DSDirLightPassTech.SetColorTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_DIFFUSE);
		m_DSDirLightPassTech.SetNormalTextureUnit(DeferredShadingGraphicsBuffer::TEXTURE_TYPE_NORMAL);
		m_DSDirLightPassTech.SetDirectionalLight(dirLight[0]);
		m_DSDirLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_DSDirLightPassTech.SetWVP(glm::mat4());

		if (!m_nullTech.Init()) {
			return false;
		}

		if (!quad.LoadMesh("../Content/quad.obj")) {
			return false;
		}

		if (!box.LoadMesh("../Content/box.obj")) {
			return false;
		}

		if (!sphere.LoadMesh("../Content/sphere.obj")) {
			return false;
		}

		if (!cone.LoadMesh("../Content/cone.obj")) {
			return false;
		}

		viewport1.directionLighTech = &m_DSDirLightPassTech;
		viewport1.pointLightTech = &m_DSPointLightPassTech;
		viewport1.geomPassTech = &m_DSGeomPassTech;
		viewport1.nullTech = &m_nullTech;
		viewport1.sphere = &sphere;
		viewport1.cone = &cone;
		viewport1.quad = &quad;

		viewport2.directionLighTech = &m_DSDirLightPassTech;
		viewport2.pointLightTech = &m_DSPointLightPassTech;
		viewport2.geomPassTech = &m_DSGeomPassTech;
		viewport2.nullTech = &m_nullTech;
		viewport2.sphere = &sphere;
		viewport2.cone = &cone;
		viewport2.quad = &quad;
#ifndef WIN32
		if (!m_fontRenderer.InitFontRenderer()) {
			return false;
		}
#endif
		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{
		CalcFPS();

		for (int i = 0; i < 5; ++i)
		{
			boxNode[i].rotation = glm::rotate(boxNode[i].rotation, scale, glm::vec3(0, 1, 0));
		} //for

		scene1.render();

		RenderFPS();
		glutSwapBuffers();
	}

	void KeyboardCB(OGLDEV_KEY OgldevKey)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		default:
			return;
			//m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		//camera->OnMouse(x, y);
	}

	// The calculation solves a quadratic equation (see http://en.wikipedia.org/wiki/Quadratic_equation)
	static float CalcPointLightBSphere(const PointLight& Light)
	{
		float MaxChannel = fmax(fmax(Light.color.x, Light.color.y), Light.color.z);

		float ret = (-Light.attenuation.linear + sqrtf(Light.attenuation.linear * Light.attenuation.linear - 4 * Light.attenuation.exponential * (Light.attenuation.exponential - 256 * MaxChannel * Light.diffuseIntensity)))
			/
			2 * Light.attenuation.exponential;

		return ret * 1.6;
	}

	static float calcSpotLightBCone(const SpotLight& Light)
	{
		return CalcPointLightBSphere(Light);
	} //calcSpotLightBCone


private:
	void InitLights()
	{
		dirLight[0].ambientIntensity = 0.1f;
		dirLight[0].color = glm::vec3(0, 1, 1);
		dirLight[0].diffuseIntensity = 0.5f;
		dirLight[0].direction = glm::vec3(1.0f, 0.0f, 0.0f);

		pointLight[0].diffuseIntensity = 0.2f;
		pointLight[0].color = glm::vec3(0, 1, 0);
		pointLight[0].position = glm::vec3(0.0f, 1.5f, 5.0f);
		pointLight[0].attenuation.constant = 0.0f;
		pointLight[0].attenuation.linear = 0.0f;
		pointLight[0].attenuation.exponential = 0.3f;
		pointLight[0].scale = glm::vec3(CalcPointLightBSphere(pointLight[0]));

		pointLight[1].diffuseIntensity = 0.2f;
		pointLight[1].color = glm::vec3(1, 0, 0);
		pointLight[1].position = glm::vec3(2.0f, 0.0f, 5.0f);
		pointLight[1].attenuation.constant = 0.0f;
		pointLight[1].attenuation.linear = 0.0f;
		pointLight[1].attenuation.exponential = 0.3f;
		pointLight[1].scale = glm::vec3(CalcPointLightBSphere(pointLight[1]));

		pointLight[2].diffuseIntensity = 0.2f * 100;
		pointLight[2].color = glm::vec3(0, 0, 1);
		pointLight[2].position = glm::vec3(0.0f, 0.0f, -5.0f);
		pointLight[2].attenuation.constant = 0.0f;
		pointLight[2].attenuation.linear = 0.0f;
		pointLight[2].attenuation.exponential = 0.3f;
		pointLight[2].scale = glm::vec3(CalcPointLightBSphere(pointLight[2]));
	}


	void InitBoxPositions()
	{
		boxNode[0].position = glm::vec3(0.0f, 0.0f, 5.0f);
		boxNode[1].position = glm::vec3(0.0f, 5.0f, 5.0f);
		boxNode[2].position = glm::vec3(0.0f, -5.0f, 5.0f);
		boxNode[3].position = glm::vec3(0.0f, 0.0f, 10.0f);
		boxNode[4].position = glm::vec3(0.0f, 5.0f, 10.0f);

		boxNode[0].mesh = &box;
		boxNode[1].mesh = &box;
		boxNode[2].mesh = &box;
		boxNode[3].mesh = &box;
		boxNode[4].mesh = &box;
	}
};


int main(int argc, char** argv)
{
	Magick::InitializeMagick(*argv);
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 37")) {
		return 1;
	}

	Tutorial37* pApp = new Tutorial37();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}