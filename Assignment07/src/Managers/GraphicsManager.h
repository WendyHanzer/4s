#pragma once

/*
*	missing includes for SFML
*/

//http://sfml-dev.org/tutorials/2.0/window-opengl.php
//http://sfml-dev.org/documentation/2.0/classsf_1_1RenderWindow.php

/*
*	one Window for each active scene
*
*	If reducing number of active scenes, close a window
*	If increasing number of active scenes, add a window
*	Each scene has window settings. These settings are copied to the window that the scene will use.
*	Everything rendered from the scene is rendered to the window it is using.
*	GraphicsManager should correctly render all active scenes, swapping between windows
*	This is easy to do with eventRender. Whenever a scene starts rendering, the scene itself will set the active window, not the GraphcisManager
*
*	SceneManager
*		fire render event
*
*	Active Scene 1
*		set active window
*		fire render event
*
*	Active Scene 2
*		set active window
*		fire render event
*
*	Active Scene 3
*		set active window
*		fire render event
*
*	The work here is in connecting scenes up with windows
*	When a scene unloads, it should release a window
*	When a scene loads, it should request a window
*	A window should be closed after a scene transition if that window is no longer in use
*	The window size should be scaled by the ratio between the old scene settings and new scene settings
*	-	this will maintain window resizes from users
*
*	The graphics manager sets the ShaderProgram up and so forth, then fires the render event, then unsets it.
*	It's the main loop. The window stuff goes inside. Every GUI might be on its own window too!!! This means that
*	scenes need to have GUIs. A GUI can be thought of as a layer. Scenes should be able to share GUIs too.
*
*	WindowManager? Each window has its own settings, scenes go through windows? So rather than a scene requesting a window, a window
*	is set to a scene. When making a new active scene, it needs to be bound to a window. It falls on the user to manage this.
*	A scene has no width/height settings, a window does! This means that each window is rendered, which in turn happens to render
*	the scene on the window.
*
*	Window1
*		set active window
*		fire render event
*
*		Scene on window
*			receive render event
*			fire render event
*
*	Window2
*		etc...
*
*	Should the GraphicsManager be in charge of these windows? For example, addWindow? Probably. Should it loop through them? No.
*	What about windows that just have different viewing angles? 2 windows can have the same exact scene, but they may have different views
*	and projections.
*
*	Camera on window vs Camera on scene. Where should the camera be? Perhaps each scene has a default camera, but the camera is located on the
*	window. The scene's default camera settings are copied to the window's camera. The window's camera is what controls things. The window camera
*	can then be overrided with different settings, for example, different views.
*
*	A window has the projection matrix. A camera has the view matrix.
*
*	Window has layers
*
*		2D layer
*		3D layer
*
*	These layers are drawn in order from top to bottom
*
*	Example
*
*		Background (2D GUI)
*		Scene (3D Scene)
*		Foreground (2D GUI)
*
*	Things can be put into a specific layer. A scene may be swapped out from the scene layer for another scene. This means that
*	a window should not have a render event, rather it should iterate through all layers and fire rendering events for those layers
*	directly. This will maintain correct order.
*
*	Window
*
*		onRender //registered to graphics eventRender
*
*			vector<Layer> layers = {Layer2D/Layer3D, ...}
*
*			for (Layer* layer : layers) { layer.render(); }
*			//	render would push/pop OpenGL states and do set up
*			//	each 3D layer would have its own shader and so on
*			//	2D layers are handled by SFML
*
*	GraphicsManager
*
*		onTick
*
*			eventRender.fire()
*
*	So perhaps instead of having GraphicsManager manage windows itself, there should be a WindowManager. When a scene loads, how does it
*	know which window to go to? Figure out how to send a scene to the correct window of the correct layer.
*
*	Within the SceneManager, think about different types of scenes. When a scene transitions to another scene, that new scene should
*	inherit the old scene's window and layer. It would just be a swap. This way you don't have to track everything. You set things up
*	initially with everything in the right position and then let it run. If something goes wrong, then a bad swap took place, like a
*	scene getting swapped out for a 2D menu. This will simplify management.
*
*	When a scene gets swapped in, it registers to the layer's render event. The same goes for a GUI.
*
*	A scene should have the layer it is bound to (Layer* layer). A Layer should have the window it belongs to (Window* window). This is
*	not an SFML window, but our own window. This window will include an SFML window inside of it.
*
*	Be sure to use sf::RenderWindow, not sf::Window.
*
*	When I have time, I'll make a diagram of this design.
*/

#include <string>
#include <vector>

#include "Manager.h"

#include <TGUI/TGUI.hpp>

/*
*	this class
*
*		sets up rendering
*		runs render event
*		cleans up rendering
*
*		renders to a target window
*/

/*
*	GraphicManager requires SceneManager
*/
class GraphicManager : public Manager { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			void onTick(double delta);
			//runs under SceneManager::eventTick
			//runs eventRender
/*		Constructors
*		----------------------------
*/
			GraphicManager(Engine* engine);
			virtual ~GraphicManager();
/*
*****************************************************************
*
*/	public: /*
*
*
*		Fields
*		----------------------------------
*/
			Event<void> eventRender;
			Event<void> eventStart;
			Event<void> eventStop;
			Event<void> eventDestroy;

/*
*		Properties
*		----------------------------------
*/


/*
*****************************************************************/

}; //GraphicsManager
