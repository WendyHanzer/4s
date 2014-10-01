OpenGL Assimp
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
	
Description

	This will load an obj using assimp. The only reason it is currently bound to obj is because of the way it scans for arguments.
	
	This does use glDrawElements (indexed drawing) instead of glDrawArrays.
	
	Texture loading is currently broken.
		
Running

	Read instructions in program

	The program takes n optional arguments (order does not matter)

		vertex shader file name			-	filename.vs
		fragment shader file name		-	filename.fs
		obj file name					-	filename.obj
		scale							-	value.scale
		
	The program requires 1+ objs to be passed in in order to run
		
	Shaders will be coupled in the order that they are found
	
	Example
	
		fs1 fs2 vs1 fs3 vs2 vs3 -> {fs1, vs1}, {fs2, vs2}, {fs3, vs3}
		
	Only the first scaling value is considered. When scale is passed into the program, it is applied to everything. When desiring scales for single models, work within the program or do some
	sort of ini for the model to define a scale for it. Scaling goes as follows: global + model + entity
		
	For now, only the first encounters are considered. The rest of the shaders and objs are ignored.
	
	Default shaders are supplied (.fs and .vs). These will be used if no shaders are put into the arguments.
