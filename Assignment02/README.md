Menus in OpenGL
========================================

Requirements

	SFML
		~$ sudo apt-get install libsfml-dev

	GLM
		~$ sudo apt-get install libglm-dev
		
	TGUI
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
Build

	cd build
	make
	
Clean

	make clean
