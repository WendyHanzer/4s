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
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include <math.h>

#include "shader.h"
#include "program.h"

/*
*	rendering is a total mess, will clean it up more for next assignment
*/

struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};

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
	}
	
	void project(const glm::mat4 model, glm::mat4& mvp)
	{
		mvp = projection*view*model;
	};
	
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
	}
};

class Model
{
public:
	glm::mat4 model;
};

void loadWidgets(tgui::Gui& gui, tgui::Gui& gui2, sf::RenderWindow& window);
void loadGeometry(GLuint& vbo_geometry);
void cleanUp(GLuint& vbo_geometry, sf::RenderWindow& window);
void update(Model& model, const float rotationRate, const float translationRate, const float dt);
void render(const Model& model, const Camera& camera, const Program& program);

bool simPaused = false;

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
	GLuint vbo_geometry;
	loadGeometry(vbo_geometry);
	
	/*
	*	load shader
	*/
	Shader shader;
	if (!shader.loadShadersFile(argc >= 2? argv[1] : 0, argc >= 3? argv[2] : 0))
	{
		window.close();
		
		return -1;
	} //if
	
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
	
	Model model;
	
	sf::Clock clock;
	
	/*
	*	main loop
	*/
	//spin is the object spinning about its y-axis
	//rotation is the object translating around the origin in a circle
	float spinRate = M_PI;
	float rotationRate = M_PI/2;
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
							cleanUp(vbo_geometry, window);
							exit(0);
							
							break;
						case sf::Keyboard::Up:
							if (!simPaused)
							{
								rotationRate += 0;	//if wanting to modify rotation
								spinRate += .1;
							}
							break;
						case sf::Keyboard::Down:
							if (!simPaused)
							{
								rotationRate -= 0;	//if wanting to modify rotation
								spinRate -= .1;
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
							//rotationRate *= -1;	//if wanting to modify rotation
							spinRate *= -1;
						
							break;
						default:
							break;
					} //switch
				default:
					break;
			} //switch
		
			gui.handleEvent(event);
		} //if
	
		while (gui.pollCallback(callback))
		{
			gui.handleEvent(event);
		} //if
		
		update(model, spinRate, rotationRate, clock.restart().asSeconds());
		
		window.clear();
		
		window.pushGLStates();
		gui2.draw();
		window.popGLStates();
		
		glClear(GL_DEPTH_BUFFER_BIT);
		
		/*
		*	render OpenGL here
		*/
		glUseProgram(program.program);
		glEnableVertexAttribArray(program.location.position);
		glEnableVertexAttribArray(program.location.color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
		
		render(model, camera, program);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(program.location.position);
		glDisableVertexAttribArray(program.location.color);
		glUseProgram(0);
		
		window.pushGLStates();
		gui.draw();
		window.popGLStates();
		
		window.display();
	} //while

    // Clean up after ourselves
    cleanUp(vbo_geometry, window);
	
	return EXIT_SUCCESS;
} //main

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
    instructions2->setText("to control speed of spin");
    instructions2->setPosition(0, 32);

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

void loadGeometry(GLuint& vbo_geometry)
{
	/*
	*	define a cube
	*/
	Vertex geometry[] = { {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},
                          
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          
                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},

                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}}
                        };

	/*
	*	create vertex buffer object
	*/
	glGenBuffers(1, &vbo_geometry);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
}

void update(Model& model, const float rotationRate, const float translationRate, const float dt)
{
    static float angle = 0;
    static float rotation = 0;
    
    if (!simPaused)
    {
		angle += dt*translationRate;
		rotation += dt*rotationRate;
	}
	
    model.model = glm::translate(glm::mat4(1.0f), glm::vec3(4*sin(angle), 0, 4*cos(angle)));
	model.model = glm::rotate(model.model, rotation, glm::vec3(0, 1, 0));
}

void render(const Model& model, const Camera& camera, const Program& program)
{
	static glm::mat4 mvp;

    //premultiply the matrix for this example
	mvp = camera.projection*camera.view*model.model;

    //upload the matrix to the shader
    glUniformMatrix4fv(program.location.mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer(program.location.position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( program.location.color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,color));

    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
}

void cleanUp(GLuint& vbo_geometry, sf::RenderWindow& window)
{
	if (window.isOpen())
    {
    	window.close();
	} //if

	glDeleteBuffers(1, &vbo_geometry);
} //cleanUp
