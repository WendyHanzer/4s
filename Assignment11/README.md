Labyrinth
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

	

Running

	Go to bin and type ./Program

	Controls:
		
		(FPS Controls)
		W:
			move forward
		A:
			strafe left
		S:
			move backward
		D:
			strafe right
			
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
			
		Up Arrow: tilt board forward
		Down Arrow: tilt back
		Right Arrow: tilt right
		Left Arrow: tilt left
			
		(Admin)
		Esc:
			exit program

Extra Credit (see report)

	*	top 10 high score
	*	sounds
	*	multiple levels

Credits

	Models
	
		http://www.turbosquid.com/FullPreview/Index.cfm/ID/725508
	
	Textures
	
		http://2.bp.blogspot.com/-uD772Sbux0g/UPbl6sz7fyI/AAAAAAAAD9Q/VCW8qyEmULY/s1600/Tileable+stone+texture+(14).jpg
		https://www.filterforge.com/filters/1316.jpg
		http://freeseamlesstextures.com/images/14-green-watercolor-background-sml.jpg
		http://freeseamlesstextures.com/images/16-red-watercolor-background-sml.jpg
		http://fc02.deviantart.net/fs37/f/2008/273/b/6/Volcano_by_AndreeWallin.jpg
		http://www.unsigneddesign.com/Seamless_background_textures/1200px/seamlesstexture12_1200.jpg
		http://jtkiwi.files.wordpress.com/2010/04/goma-volcano-trek-april-2010_-183-of-290.jpg
		http://www.bhmpics.com/view-3d_chinese_dragon-1920x1080.html
		
		The Complex - http://incompetech.com/music/royalty-free/index.html?feels%5B%5D=Epic
		Ripples		- http://incompetech.com/music/royalty-free/index.html?genre=World&page=2
		World Vice	- http://incompetech.com/music/royalty-free/index.html?genre=World&page=3
