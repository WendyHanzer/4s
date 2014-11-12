#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem(Engine& engine, sf::RenderWindow& window, tgui::Gui& layer0, tgui::Gui& layer1, tgui::Gui& layer2, Camera& camera) :
	System(engine),
	projection(glm::perspective(45.0f, float(window.getSize().x)/float(window.getSize().y), .01f, 100.0f)),
	//view(glm::lookAt(glm::vec3(0.0, 16.0, 16.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0))),			//temp
	camera(camera),
	layer0(layer0),
	layer1(layer1),
	layer2(layer2),
	window(window),
	program(NULL)
{
	GLenum status = glewInit();
	if(status != GLEW_OK)
	{
		std::cerr << "[F] GLEW NOT INITIALIZED: ";
		std::cerr << glewGetErrorString(status) << std::endl;
	
		window.close();
	
		throw 1;
	} //if
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
	//glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
   	//glCullFace(GL_BACK);
	glClearDepth(1.f);
	glShadeModel(GL_SMOOTH);
	
	glViewport(0, 0, window.getSize().x, window.getSize().y);
} //GraphicsSystem

GraphicsSystem::~GraphicsSystem()
{
	destroy();
	
	if (window.isOpen())
	{
		window.close();
	} //if
} //~GraphicsSystem

void GraphicsSystem::onTick(const long double& delta)
{
	if (!window.isOpen())
	{
		engine.stop();
		return;
	} //if
	
	window.setActive();
	window.clear();
	
	window.pushGLStates();
	layer2.draw();
	window.popGLStates();
	
	glClear(GL_DEPTH_BUFFER_BIT); 
	
	if (program)
	{
		v = camera.getView();
		vp = projection*v;
		
		std::vector<Light*> lightData[3];
		
		for (Light* light : lights)
		{
			if (lightData[light->ID].size() < 8)
			{
				lightData[light->ID].push_back(light);
			} //if
		} //for
		
		//shaders are normally per mesh as materials require specific shaders in order
		//to achieve the original look
		program->use();
		
		//lights are normally done in a scene
		//this would normally be done with deferred rendering as well
		
		for (int lightType = 0; lightType < 3; ++lightType)
		{
			if (lightData[lightType].size() > 0)
			{
				glUniform1i(program->uniform(std::string("num") + lightData[lightType][0]->NAME + std::string("s")), lightData[lightType].size());
				
				for (int light = 0; (unsigned int)light < lightData[lightType].size(); ++light)
				{
					lightData[lightType][light]->load(*program, light);
				} //for
			} //if
		} //for
		
		//glm::mat4 view = camera.getView();
		glm::vec3 cameraEye = camera.getPosition();
		//glUniformMatrix4fv(program->uniform("invV"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniform3f(program->uniform("cameraPosition"), cameraEye.x, cameraEye.y, cameraEye.z);
		
		System::onTick(delta);
		
		/*
		*	clear
		*/
		for (int lightType = 0; lightType < 3; ++lightType)
		{
			if (lightData[lightType].size() > 0)
			{
				glUniform1i(program->uniform(std::string("num") + lightData[lightType][0]->NAME + std::string("s")), 0);
			} //if
		} //for
		
		program->disable();
	} //if
	
	window.pushGLStates();
	layer1.draw();
	layer0.draw();
	window.popGLStates();
	
	window.display();
} //onTick
