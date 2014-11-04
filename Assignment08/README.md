Bullet Physics
========================================

Team

	Rui Wu
	Eli Gabay
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

	This will create the following actors with physics and graphics enabled

		Air Hockey Table
		Sphere
		Cylinder
		Cube

	The code runs on top of the base of an Engine. All of the systems being plugged into the engine
	are temporary until the correct systems can be coded. As a result, the code, except for the engine, is very messy.

	The Engine includes several behaviors and a variety of events. Basic classes extend from these behaviors to define
	a basic, but extremely flexible structure.

	The primary events are, for a complete behavior

		start
		stop
		destroy
		tick

	Entities follow a component design pattern. A given entity can be expanded to have new functionality by wrapped it
	with a component. The component can be accessed from the entity via an array.

	Entity managers define the context for a set of entities, that is, what components they are allowed to have. The primary
	purpose of this is for generating the array and mappings to the systems that dictate the execution of the components.

	Components may expand off of anything and be controlled by anything. As such, they are extremely flexible. There is a lot
	of freedom in what can be done.

	The design does not particularly allow objects from a higher level to be stored in objects of a lower level. This was done
	for this assignment as there was not enough time to do it correctly. As such, the team had to do some workarounds to get those
	higher level objects in, namely the window.

	The relationship between a Physics Component and the transformation of an Actor component (later to be SceneNode) is bidirectional.
	Whenever the actor changes position, the position present in Bullet is changed. Whenever Bullet changes position, the position in
	Actor is changed.

Running

	Buttons in program: Quit

	At initialization, the sphere, cylinder, and cube will be placed on top of the Air Hockey Table. One will initially be selected (not shown).
	The selected actor can be controlled with arrow keys (applies force).
	
	5 planes are created independet of the table. The table gets no physics component. This is done becaues when a plane is scaled, it no longer
	works. Also, the triangle meshes haven't been working very well. The problem likely lies in mesh.h when building the two vectors that contain
	the face and vertex information.
	
	The 5 planes are, like everything else, applied to an Actor. This actor has no graphics component, just a physics component.

	There is no camera. The view is constant.
	There is no indication of what actor is selected.

	Keyboard Controls:

		tab:
			change to another actor

		up:
			move selected actor forward
		down:
			move selected actor backward
		left:
			move selected actor left
		right:
			move selected actor right

Extra Credit

	None

		We were going to, but the triangle meshes are doing weird things. We have the code there to do it, but box
		was sitting halfway inside of a plane resting on one of its corners.
