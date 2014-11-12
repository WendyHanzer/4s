Bullet Physics
========================================

Team

	Aaron Solomon

Requirements

	SFML
		Makes 2D graphics easier and good for GUI

		~$ sudo apt-get install libsfml-dev

	GLM
		~$ sudo apt-get install libglm-dev

	TGUI
		Good for GUI

		Download ->	https://github.com/texus/TGUI/archive/v0.6.5.zip

		Extract

		Open Terminal in TGUI folder


		Type
			cmake .
			sudo make install -j6
			sudo ldconfig

		It is possible that the program will not run. If this occurs, it can't find TGUI. To fix, make sure
		that the library is in a place where distrobution searches for it. Move it into such a place and then
		type sudo ldconfig. I personally did not run into this issue.

		More information -> http://tgui.eu/tutorials/v06/linux/

	Boost
		This is required to build assimp

		~$ sudo apt-get install libboost-all-dev

	assimp
		Easy way to load meshes, etc

		Download assimp

			http://sourceforge.net/projects/assimp/files/assimp-3.1/assimp-3.1.1.zip/download

		Open the archive and extract the assimp folder somewhere

		Open a terminal in the assimp directory

		Type

			~$ cmake -G 'Unix Makefiles'
			~$ make
			~$ sudo make install

	Magick++
		Good texture handling

		Download the source

			http://www.imagemagick.org/script/download.php

		Open the archive and extract the ImageMagic folder (will have a version on it) somehwere

		Open a terminal in the ImageMagick directory

		Type

			~$ ./configure
			make
			sudo make install
			make check

	Bullet
		Physics engine

		~$ sudo apt-get install libbullet-dev

Build

	cd build
	make

Clean

	make clean

Description

	This does lighting about a cube with a small shininess coefficient.

Running

	Go to bin and type ./Program

	Controls:

		(Spot Light Controls)
		tab:
			change selected spot light
		up:
			change direction of selected spot light
		down:
			change direction of selected spot light
		left:
			change direction of selected spot light
		right:
			change direction of selected spot light
		space:
			toggle selected spot light on/off
		enter:
			add spot light to scene
			
		(General Light Controls)
		1:
			toggle ambient light on/off
		2:
			toggle directional light on/off
		3:
			toggle point light on/off
		
		(FPS Controls)
		W:
			move forward
		A:
			strafe left
		S:
			move backward
		D:
			strafe right
		Mouse Movement:
			face based on mouse delta
		---------------------------------
		R:
			face upwards
		F:
			face downwards
		Q:
			face left
		E:
			face right
		Page Up:
			move vertically up (along y-axis)
		Page Down:
			move vertically down (along y-axis)
			
		(Admin)
		Esc:
			exit program
			
	Light Locations (not necessarily exact locations):
	
		Spotlight is located at the bottom of the cube (move below the cube)
		Point light is located above the cube (move to the top of the cube)
		Directional light is facing the front of the cube
		
Issues:

	1.	8 lights of each type are currently supported. Ambient light is just a directional light
	with no diffuse lighting. However, when having more than 2 spotlights, the later ones don't
	show up. Too tired to figure out why. However, in technicality, it is still possible to add
	1 spotlight (and more, they will just not do anything), so /win.
	
	2.	Will likely move to deferred rendering later if there is ever time to do so.
	
	3.	The normal matrix, when multiplied with the normal vector from the mesh, makes it
	always perpendicular to the light direction... The model matrix is fine. Gave up after
	10 hours of trying to figure out why. Guess I'm not using scaling for the time being, lol.
	
	4.	The directional light loves to shine on the right face of the cube along with the front when
	the camera approaches the right face. Very strange. The other faces are ok.
	
	5.	Currently using constants for the material diffuse, specular, ambient, and shininess
	properties as many of the objects I've been testing didn't have them. Also set assimp
	to automatically calculate normals to use this cube that has no normals.

Extra Credit

	*	Implement controls to change the direction of the spot light
	*	Allow for the addition of spotlights and their individual toggeling and controls.
