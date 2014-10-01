/*
*	decent example of OpenGL with SFML
*
*		https://github.com/LaurentGomila/SFML/blob/master/examples/opengl/OpenGL.cpp
*
*	Not sure if the thing wanted to handle both translation and rotation or just one.
*	Given that the word "spinning" was used, I will assume just one. Either way, it shows
*	that I know the material.
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <TGUI/TGUI.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <math.h>

#include "shader.h"
#include "shaderProgram.h"
#include "mesh.h"		//this is currently not used as it doesn't work yet

/*
*	rendering is a total mess, will clean it up more for next assignment
*/

class Camera
{
public:
	glm::mat4 view;			//world->eye
	glm::mat4 projection;	//eye->clip
	
	float fieldOfView;
	float nearPlane;
	float farPlane;
	float aspectRatio;
	
	glm::vec3 eyePosition;
	glm::vec3 focusPoint;
	glm::vec3 upDirection;
	
	void updateView()
	{
		view = glm::lookAt(eyePosition, focusPoint, upDirection);
	} //updateView
	
	void updateProjection()
	{
		projection = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);	
	} //updateProjection
	
	void project(const glm::mat4& model, glm::mat4& mvp)
	{
		mvp = projection*view*model;
	} //project
	
	Camera()
	{
		fieldOfView = 45.0f;
		nearPlane = 0.01f;
		farPlane = 100.0f;
		aspectRatio = 800.0f/600.0f;
		
		eyePosition = glm::vec3(0.0, 8.0, -16.0);
		focusPoint = glm::vec3(0.0, 0.0, 0.0);
		upDirection = glm::vec3(0.0, 1.0, 0.0);
		
		updateView();
		updateProjection();
	} //constructor
}; //Camera

class Model
{
public:
	glm::vec3 scale;
	GLuint vbo_geometry;
	int triangles;
	Mesh* mesh;
	
	Model()
	{
		vbo_geometry = 0;
		mesh = 0;
		triangles = 0;
	} //constructor
	
	~Model()
	{
		if (mesh)
		{
			delete mesh;
		} //if
		
		if (vbo_geometry)
		{
			glDeleteBuffers(1, &vbo_geometry);
		} //if
	}
}; //Model

class Planet;

vector<Planet*> planets;
int focusedPlanet = 0;

class Planet
{
public:
	Model* model;
	
	glm::mat4 position;
	list<Planet*> moons;
	
	Planet(Model* model)
	{
		this->model = model;
		
		planets.push_back(this);
	}
}; //Planet

void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2, sf::RenderWindow& window);
void render(Planet& planet, Camera& camera, ShaderProgram& program);
void guiDraw(sf::RenderWindow& window, tgui::Gui& gui);

void getArgs(int count, char** files, const char* extension, char** args)
{
	int index = 0;
	
	int len = 0;
	args[0] = NULL;
	
	while (count > 0)
	{
		index = std::strlen(*files);
		
		while (index != 0 && (*files)[index] != '.')
		{
			--index;
		} //while
		
		if (!std::strcmp((*files) + index, extension))
		{
			args[len++] = *files;
			args[len] = 0;
		} //if
		
		--count;
		++files;
	} //while
} //getArgs

int main(int argc, char** argv)
{
	--argc;
	++argv;

	const int WIDTH = 800;
	const int HEIGHT = 600;

	/*
	*	Create window
	*/
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Window", sf::Style::Default, sf::ContextSettings(32));
	
	/*
	*	Initialize GLEW
	*/
	GLenum status = glewInit();
	if(status != GLEW_OK)
	{
		std::cerr << "[F] GLEW NOT INITIALIZED: ";
		std::cerr << glewGetErrorString(status) << std::endl;
	
		window.close();
	
		return -1;
	}

	/*
	*	Create GUI
	*/
	tgui::Gui gui(window);
	tgui::Gui gui2(window);
	gui.setGlobalFont("fonts/DejaVuSans.ttf");
	loadWidgets(gui, gui2, window);
	
	char* args[argc + 1];
	
	/*
	*	load geometry
	*/
	Model cube;
	getArgs(argc, argv, ".obj", args);
	
	if (!args[0])
	{
		std::cerr << "[F] MUST SUPPLY 1+ OBJ FILES IN COMMAND LINE ARGS <filename.obj>" << std::endl;
		exit(-1);
	} //if
	
	cube.mesh = loadMesh(args[0]);
	
	getArgs(argc, argv, ".scale", args);
	if (args[0])
	{
		cube.scale = glm::vec3(strtof(args[0], NULL));
	} //if
	else
	{
		cube.scale = glm::vec3(1.0f);
	} //else
	
	/*
	*	load shaders
	*/
	Shader vertexShader(GL_VERTEX_SHADER);
	getArgs(argc, argv, ".vs", args);
	vertexShader.loadFromFile(args[0]? args[0] : ".vs");
	vertexShader.compile();
	
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	getArgs(argc, argv, ".fs", args);
	fragmentShader.loadFromFile(args[0]? args[0] : ".fs");
	fragmentShader.compile();
	
	/*
	*	create program
	*/
	ShaderProgram program;
	
	program.attachShader(vertexShader);
	program.attachShader(fragmentShader);
	program.linkProgram();
	
	program.addAttribute("vertexPosition_modelspace");
	program.addAttribute("vertexUV");
	//program.addAttribute("vertexNormal_modelspace");
	
	program.addUniform("MVP");
	program.addUniform("myTextureSampler");
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glClearDepth(1.f);
	glShadeModel (GL_SMOOTH);
	
	glUniform1i(program.uniform("myTextureSampler"), 0);

	Camera camera;
	
	sf::Event event;
	tgui::Callback callback;
	
	Planet p(&cube);
	
	/*
	*	main loop
	*/
	
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
				
					break;
				case sf::Event::Resized:
					glViewport(0, 0, event.size.width, event.size.height);
					camera.projection = glm::perspective(45.0f, float(event.size.width)/float(event.size.height), 0.01f, 100.0f);
					
					break;
				default:
					break;
			} //switch
			
			gui.handleEvent(event);
		} //if
		
		while (gui.pollCallback(callback))
		{
			gui.handleEvent(event);
		} //if
		
		window.clear();
		guiDraw(window, gui2);
		glClear(GL_DEPTH_BUFFER_BIT); 
		
		/*
		*	render OpenGL here
		*/
		//glValidateProgram(program.program);
		
		for (Planet* planet : planets)
		{
			render(*planet, camera, program);
		} //for
		
		guiDraw(window, gui);
		
		window.display();
	} //while

	// Clean up after ourselves
	if (window.isOpen())
	{
		window.close();
	} //if
	
	return EXIT_SUCCESS;
} //main

void guiDraw(sf::RenderWindow& window, tgui::Gui& gui)
{
	window.pushGLStates();
	gui.draw();
	window.popGLStates();
} //guiDraw

void quitCallback(const tgui::Callback& callback)
{
	exit(0);
}

void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2, sf::RenderWindow& window)
{
	tgui::Picture::Ptr picture(gui2);
	picture->load("images/xubuntu_bg_aluminium.jpg");
	picture->setCallbackId(3);

	//would be nice to get below to work, but OpenGL covers it
	//picture->bindCallbackEx(onBackClick, tgui::Picture::LeftMouseClicked);
	
	// Create the quit button
	tgui::Button::Ptr quit(gui);
	quit->load("widgets/Black.conf");
	quit->setSize(128, 30);
	quit->setPosition(window.getSize().x - 128, 0);
	quit->setText("Quit");
	quit->setCallbackId(2);
	quit->bindCallbackEx(quitCallback, tgui::Button::LeftMouseClicked);
}

void render(Planet& planet, Camera& camera, ShaderProgram& program)
{
	planet.model->mesh->render(program, camera.projection*camera.view*glm::scale(planet.position, planet.model->scale));
}
