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
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <math.h>

#include "shader.h"
#include "program.h"
#include "mesh.h"		//this is currently not used as it doesn't work yet
#include "objLoader.h"

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
	
	void project(const glm::mat4 model, glm::mat4& mvp)
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
	
	float rotationRate;
	float translationRate;
	float rotation;
	float translation;
	
	Planet(Model* model)
	{
		this->model = model;
	
		rotation = 0;
		translation = 0;
		translationRate = M_PI/2;
		rotationRate = M_PI;
		
		planets.push_back(this);
	}
}; //Planet

void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2, sf::RenderWindow& window);
void loadGeometry(Model& model, const char* filename);
void update(Planet& planet, const float dt, Planet* parent);
void render(const Planet& planet, const Camera& camera, const Program& program);
void guiDraw(sf::RenderWindow& window, tgui::Gui& gui);

bool simPaused = false;

queue<char*>& getExtensions(int count, char** files, const char* extension)
{
	queue<char*>& ext = *new queue<char*>();
	int index = 0;
	
	while (count > 0)
	{
		index = std::strlen(*files);
		
		while (index != 0 && (*files)[index] != '.')
		{
			--index;
		} //while
		
		if (!std::strcmp((*files) + index, extension))
		{
			ext.push(*files);
		} //if
		
		--count;
		++files;
	} //while
	
	return ext;
} //getExtensions

int main(int argc, char** argv)
{
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
	
	/*
	*	load geometry
	*/
	Model cube;
	queue<char*>& objs = getExtensions(argc - 1, argv + 1, ".obj");
	
	if (objs.empty())
	{
		std::cerr << "[F] MUST SUPPLY 1+ OBJ FILES IN COMMAND LINE ARGS <filename.obj>" << std::endl;
	
		window.close();
		
		delete &objs;
	
		return -1;
	} //if
	
	loadGeometry(cube, objs.front());
	
	delete &objs;
	
	queue<char*>& scales = getExtensions(argc - 1, argv + 1, ".scale");
	if (!scales.empty())
	{
		cube.scale = glm::vec3(strtof(scales.front(), NULL));
	} //if
	else
	{
		cube.scale = glm::vec3(1.0f);
	} //else
	delete &scales;
	
	/*
	*	load shader
	*/
	queue<char*>& vertexShaders = getExtensions(argc - 1, argv + 1, ".vs");
	queue<char*>& fragmentShaders = getExtensions(argc - 1, argv + 1, ".fs");
	Shader shader;
	if (!shader.loadShadersFile(vertexShaders.empty()? 0 : vertexShaders.front(), fragmentShaders.empty()? 0 : fragmentShaders.front()))
	{
		window.close();
		
		return -1;
	} //if
	
	delete &fragmentShaders;
	delete &vertexShaders;
	
	/*
	*	create program
	*/
	Program program;
	if (!program.loadShader(shader))
	{
		window.close();
		
		return -1;
	} //if
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	Camera camera;
	
	sf::Event event;
	tgui::Callback callback;
	
	Planet p(&cube);
	//Planet p2(&cube);
	
	//p.moons.push_back(&p2);
	
	p.rotationRate /= 1.5;
	//p2.translationRate *= 2;
	
	sf::Clock clock;
	
	/*
	*	main loop
	*/
	//broken
	//model.mesh = loadMesh("models/cube.obj");
	
	tgui::Label::Ptr direction1(gui);
	direction1->setText("Selected- translation=NA");
	direction1->setPosition(0, 600 - 64);
	tgui::Label::Ptr direction2(gui);
	direction2->setText("          rotation=NA");
	direction2->setPosition(0, 600 - 32);
	
	auto time = clock.restart().asSeconds();
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
				case sf::Event::KeyPressed:
					switch(event.key.code)
					{
						case sf::Keyboard::Escape:
							window.close();
							exit(0);
							
							break;
						case sf::Keyboard::Up:
							if (!simPaused)
							{
								planets[focusedPlanet]->rotationRate += .1;
							}
							break;
						case sf::Keyboard::Down:
							if (!simPaused)
							{
								planets[focusedPlanet]->rotationRate -= .1;
							}
							break;
						case sf::Keyboard::Right:
							if (!simPaused)
							{
								planets[focusedPlanet]->translationRate -= .1;
							}
							break;
						case sf::Keyboard::Left:
							if (!simPaused)
							{
								planets[focusedPlanet]->translationRate += .1;
							}
							break;
						case sf::Keyboard::Tab:
							if (!simPaused)
							{
								focusedPlanet += 1;
								focusedPlanet %= planets.size();
							}
							break;
						default:
							break;
					} //switch
					
					break;
				case sf::Event::MouseButtonPressed:
					switch (event.key.code)
					{
						case sf::Mouse::Left:
							if (!simPaused)
							{
								planets[focusedPlanet]->rotationRate *= -1;
							} //if
							
							break;
						case sf::Mouse::Right:
							if (!simPaused)
							{
								planets[focusedPlanet]->translationRate *= -1;
							} //if
							break;
						default:
							break;
					} //switch
					
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
		
		time = clock.restart().asSeconds();
		update(*planets[0], time, NULL);
		
		window.clear();
		guiDraw(window, gui2);
		glClear(GL_DEPTH_BUFFER_BIT); 
		
		/*
		*	render OpenGL here
		*/
		glValidateProgram(program.program);
		
		for (Planet* planet : planets)
		{
			render(*planet, camera, program);
		} //for
		
		if (planets[focusedPlanet]->translationRate == 0)
		{
			direction1->setText("Selected- translation=NA");
		} //if
		else if (planets[focusedPlanet]->translationRate < 0)
		{
			direction1->setText("Selected- translation=clockwise");
		} //if
		else
		{
			direction1->setText("Selected- translation=counterclockwise");
		} //else
		
		if (planets[focusedPlanet]->rotationRate == 0)
		{
			direction2->setText("Selected- rotation=NA");
		} //if
		else if (planets[focusedPlanet]->rotationRate < 0)
		{
			direction2->setText("Selected- rotation=clockwise");
		} //if
		else
		{
			direction2->setText("Selected- rotation=counterclockwise");
		} //else
		
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

void pauseCallback(const tgui::Callback& callback)
{
	simPaused = callback.checked;
}

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
	
	tgui::Label::Ptr instructions1(gui);
	instructions1->setText("Use up and down arrow keys");
	instructions1->setPosition(0, 0);
	
	tgui::Label::Ptr instructions2(gui);
	instructions2->setText("to control speed of rotation");
	instructions2->setPosition(0, 32);
	
	tgui::Label::Ptr instructions3(gui);
	instructions3->setText("Use left and right arrow keys");
	instructions3->setPosition(0, 64);
	tgui::Label::Ptr instructions4(gui);
	instructions4->setText("to control speed of translation");
	instructions4->setPosition(0, 96);
	tgui::Label::Ptr instructions5(gui);
	instructions5->setText("left click to reverse rotation");
	instructions5->setPosition(0, 128);
	tgui::Label::Ptr instructions6(gui);
	instructions6->setText("right click to reverse translation");
	instructions6->setPosition(0, 160);
	tgui::Label::Ptr instructions7(gui);
	instructions7->setText("tab to change selection of planet");
	instructions7->setPosition(0, 192);

	// Create check box for pausing
	tgui::Checkbox::Ptr checkbox(gui);
	checkbox->load("widgets/Black.conf");
	checkbox->setPosition(window.getSize().x - 128, 30);
	checkbox->setText("Paused");
	checkbox->setSize(32, 32);
	checkbox->setCallbackId(1);
	checkbox->bindCallbackEx(pauseCallback, tgui::Checkbox::LeftMouseClicked);

	// Create the quit button
	tgui::Button::Ptr quit(gui);
	quit->load("widgets/Black.conf");
	quit->setSize(128, 30);
	quit->setPosition(window.getSize().x - 128, 0);
	quit->setText("Quit");
	quit->setCallbackId(2);
	quit->bindCallbackEx(quitCallback, tgui::Button::LeftMouseClicked);
}

void loadGeometry(Model& model, const char* filename)
{
	/*
	*	define a cube
	*/
	//position, texture, color, normal
	
	Obj* obj = new Obj(filename);
	
	if (!obj->valid)
	{
		delete obj;
		
		return;
	} //if
	
	/*
	*	create vertex buffer object
	*/
	
	/*
	*	this creates 1 object
	*
	*	the handles are put into vbo_geometry
	*/
	glGenBuffers(1, &model.vbo_geometry);
	
	
	/*
	*	this states that the buffer is an array of verticies
	*
	*	GL_ELEMENT_ARRAY_BUFFER would state that the buffer contains elements of
	*	verticies in another buffer
	*
	*	recall that vbo_geometry represents the buffer
	*/
	glBindBuffer(GL_ARRAY_BUFFER, model.vbo_geometry);
	
	/*
	*	this will load the last buffer with an array using GL_STATIC_DRAW as the draw method
	*/
	glBufferData(GL_ARRAY_BUFFER, obj->vertices.size()*sizeof(Vertex), obj->vertices.data(), GL_STATIC_DRAW);
	
	model.triangles = obj->vertices.size();
	
	delete obj;
}

void update(Planet& planet, const float dt, Planet* parent)
{
	if (!simPaused)
	{
			planet.rotation += dt*planet.rotationRate;
			planet.translation += dt*planet.translationRate;
	}
	
	glm::mat4 parentPosition = parent == 0? glm::mat4(1.0) : parent->position;
	
	planet.position = glm::translate(parentPosition, glm::vec3(4*sin(planet.translation), 0, 4*cos(planet.translation)));
	
	for (Planet* moon : planet.moons)
	{
		update(*moon, dt, &planet);
	} //for
	
	planet.position = glm::rotate(planet.position, planet.rotation, glm::vec3(0, 1, 0));
	planet.position = glm::scale(planet.position, planet.model->scale);
}

void render(const Planet& planet, const Camera& camera, const Program& program)
{
	static glm::mat4 mvp;
	
	glUseProgram(program.program);
	
	/*
	*	shaders have various attributes for vertices
	*	these functions will enable attributes for each vertex
	*	if attributes are not enabled, they can't be accessed by the pipeline
	*/
	glEnableVertexAttribArray(program.location.position);
	glEnableVertexAttribArray(program.location.color);
	
	/*
	*	this sets target buffer to current active buffer!
	*	buffers store vertices
	*/
	glBindBuffer(GL_ARRAY_BUFFER, planet.model->vbo_geometry);
	
	//premultiply the matrix for this example
	mvp = camera.projection*camera.view*planet.position;

	//upload the matrix to the shader
	glUniformMatrix4fv(program.location.mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

	/*
	*	these calls state how to interpret data inside the buffer
	*/
	//set pointers into the vbo for each of the attributes(position and color)
	//position, texture, color, normal
	glVertexAttribPointer(	program.location.position,			//location of attribute
							3,									//number of elements
							GL_FLOAT,							//type
							GL_FALSE,							//normalized?
							sizeof(Vertex),						//stride
							(void*)offsetof(Vertex,position));	//offset

	glVertexAttribPointer(	program.location.color,
							4,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*)offsetof(Vertex,color));

	glDrawArrays(GL_TRIANGLES, 0, planet.model->triangles); //mode, starting index, count
	//broken
	//model.mesh->render(program);
	
	//unload buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//disable shader program attributes
	glDisableVertexAttribArray(program.location.position);
	glDisableVertexAttribArray(program.location.color);
	
	//unload shader program
	glUseProgram(0);
}
