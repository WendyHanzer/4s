Menus in OpenGL
========================================

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
			
Build

	cd build
	make
	
Clean

	make clean
		
Running

	Read instructions in program

	This color cube will be rotating about the y-axis

	The program takes two optional arguments

		Argument 1: vertex shader file name
		Argument 2: fragment shader file name
	
	If no arguments are supplied, the program will use the default filenames of vs and fs respectively. The two
	necessary files are provided under default names for convenience.
