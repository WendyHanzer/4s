# Linux
CC=g++
INCLUDE=-I/usr/local/include/ImageMagick-6
OPTIONS= -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=0
LIBS= -lGL -lGLEW -ltgui -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lassimp -lMagickCore -lMagick++ -lMagickWand
PROGRAM_NAME=Program

# For Macs uncomment the next line and comment out the previous one
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++
# Assuming you want to use a recent compiler

# Compiler flags
CXXFLAGS= -g -Wall -std=c++11

all: ../bin/$(PROGRAM_NAME)

../bin/$(PROGRAM_NAME): ../src/main.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OPTIONS) ../src/main.cpp -o ../bin/$(PROGRAM_NAME) $(LIBS)
	
clean:
	rm -rf *.o ../bin/$(PROGRAM_NAME)
