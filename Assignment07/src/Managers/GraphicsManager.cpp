// Header files
#include <GraphicsManager.h>

#include <GL/glew.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Exception handling class
class GMConstructorException: public exception
{
  virtual const char* what() const throw()
  {
    return "An exception was thrown in the GraphicManager constructor";
  }
} GMCException;

/*
*   Constructor
*/
GraphicManager::GraphicManager(Engine* engine, sf::Window* window);
{
    // Engine variables
    //this is done from the Manager class
	this->engine = engine;
    this->type = engine->addManager(typeid(IdManager), this);
    this->numActiveScenes = 0;
    this-window = window;

	/*
	*	Initialize GLEW
	*/
	GLenum status = glewInit();
	if(status != GLEW_OK)
	{
		std::cerr << "[F] GLEW NOT INITIALIZED: ";
		std::cerr << glewGetErrorString(status) << std::endl;
	
		this->window.close();
	
		throw GMCException;
	}

    // Enable gl properties
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
    // Is line #20,21 going to be here or somewhere else?
    // Set the viewport
    // glViewport( 0, 0, w, h);
	
} // Constructor

//this file does anything about graphics
//including creating windows and glew!