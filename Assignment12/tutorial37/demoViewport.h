#pragma once

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>

#include <iostream>

#include <GL/glew.h>

#include "graphicsFrame.h"
#include "cameraNode.h"
#include "geometryNode.h"
#include "pointLight.h"
#include "viewport.h"
#include "spotLight.h"

#include "ds_dir_light_pass_tech.h"
#include "ds_geom_pass_tech.h"
#include "ds_light_pass_tech.h"
#include "ds_point_light_pass_tech.h"
#include "null_technique.h"

//due to lack of time, a generic buffer class was created and put into the viewport
//originally it was going to be very dynamic, adding passes freely

//supported by ogldev


//threw in general rendering too...

class DeferredShadingGraphicsBuffer
{
public:
	enum TEXTURE_TYPE {
		TEXTURE_TYPE_POSITION,
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		NUM_TEXTURES
	};
private:
	GLuint fbo;
	GLuint textures[NUM_TEXTURES];
	GLuint depthTexture;
	GLuint finalTexture;
public:
	DeferredShadingGraphicsBuffer(unsigned int windowWidth, unsigned int windowHeight)
	{
		// Create the FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		// Create the gbuffer textures
		glGenTextures(sizeof(textures) / sizeof(textures[0]), textures);

		glGenTextures(1, &depthTexture);

		glGenTextures(1, &finalTexture);

		for (unsigned int i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		} //for

		// depth
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		// final
		glBindTexture(GL_TEXTURE_2D, finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Frame Buffer Object Error: " << status << std::endl;
			return;
		} //if

		// restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	} //DeferredShadingGraphicsBuffer

	~DeferredShadingGraphicsBuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(sizeof(textures) / sizeof(textures[0]), textures);
		glDeleteTextures(1, &depthTexture);
		glDeleteTextures(1, &finalTexture);
	} //DeferredShadingGraphicsBuffer

	void start()
	{
		cout << "bind" << endl;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		cout << "draw" << endl;
		glClear(GL_COLOR_BUFFER_BIT);
		cout << "clear" << endl;
	} //start

	void geometryPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		GLenum drawBuffers[] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers(sizeof(drawBuffers) / sizeof(drawBuffers[0]), drawBuffers);
	} //geometryPass

	void stencilPass()
	{
		glDrawBuffer(GL_NONE);
	} //stencilPass

	void lightPass()
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT4);

		for (unsigned int i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_TYPE_POSITION + i]);
		} //for
	} //lightPass

	void finalPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT4);
	} //finalPass
};

class DemoViewport : public Viewport
{
public:
	DeferredShadingGraphicsBuffer* buffer;
	DSDirLightPassTech* directionLighTech;
	DSPointLightPassTech* pointLightTech;
	DSGeomPassTech* geomPassTech;
	NullTechnique* nullTech;
	Mesh* sphere;
	Mesh* cone;
	Mesh* quad;

	DemoViewport()
	{
		buffer = NULL;
	} //Viewport

	virtual ~DemoViewport()
	{
		if (buffer != NULL)
		{
			delete buffer;
		} //if
	} //~Viewport

	void geometryPass(GraphicsFrame& frame)
	{
		geomPassTech->Enable();

		cout << "bind" << endl;
		buffer->geometryPass();

		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		for (unsigned int i = 0; i < frame.sceneFrame.geometry.getSize(); ++i)
		{
			cout << "draw geometry" << endl;
			geomPassTech->SetWVP(frame.VP*frame.sceneFrame.geometry[i].worldTransformation);
			geomPassTech->SetWorldMatrix(frame.sceneFrame.geometry[i].worldTransformation);
			frame.sceneFrame.geometry[i].mesh->Render();
		}

		glDepthMask(GL_FALSE);
	}

	void stencilPass(const glm::mat4& VP, const PointLightData& light)
	{
		nullTech->Enable();

		cout << "stencil pass" << endl;
		buffer->stencilPass();
		glEnable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		nullTech->SetWVP(VP*light.worldTransformation);
		sphere->Render();
	}


	void pointLightPass(GraphicsFrame& frame, PointLightData& light)
	{
		cout << "light pass" << endl;
		buffer->lightPass();

		cout << "point light tech" << endl;
		pointLightTech->Enable();
		pointLightTech->SetEyeWorldPos(glm::vec3(camera->worldPosition[3][0], camera->worldPosition[3][1], camera->worldPosition[3][2]));

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		pointLightTech->SetWVP(frame.VP*light.worldTransformation);
		pointLightTech->SetPointLight(light);

		cout << "sphere render" << endl;
		sphere->Render();
		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
	}

	void directionalLightPass()
	{
		buffer->lightPass();

		directionLighTech->Enable();
		directionLighTech->SetEyeWorldPos(glm::vec3(camera->worldPosition[3][0], camera->worldPosition[3][1], camera->worldPosition[3][2]));

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		quad->Render();

		glDisable(GL_BLEND);
	}


	void finalPass()
	{
		buffer->finalPass();
		glBlitFramebuffer(0, 0, currentWidth, currentHeight,
			0, 0, currentWidth, currentHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	virtual void render(GraphicsFrame& frame)
	{
		if (camera != NULL)
		{
			int width = getWidth();
			int height = getHeight();
			int x = getX();
			int y = getY();

			if (width <= 0 || height <= 0)
			{
				return;
			} //if

			if (currentWidth != width || currentHeight != height || currentX != x || currentY != y)
			{
				currentWidth = width;
				currentHeight = height;
				currentX = x;
				currentY = y;

				if (buffer != NULL)
				{
					delete buffer;
				} //if

				buffer = new DeferredShadingGraphicsBuffer(currentWidth, currentHeight);
			} //if

			activate();

			cout << "start" << endl;
			buffer->start();

			cout << "geo" << endl;
			geometryPass(frame);

			cout << "point light" << endl;
			glEnable(GL_STENCIL_TEST);
			for (int i = 0; i < frame.sceneFrame.pointLight.getSize(); ++i)
			{
				stencilPass(frame.VP, frame.sceneFrame.pointLight[i]);
				pointLightPass(frame, frame.sceneFrame.pointLight[i]);
			}
			glDisable(GL_STENCIL_TEST);

			cout << "directional light" << endl;
			directionalLightPass();

			cout << "final pass" << endl;
			finalPass();
		} //if;
	} //render
}; //Viewport