# Linux
CC=g++
LIBS= -lGL -lGLEW -ltgui -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
PROGRAM_NAME=Program

# For Macs uncomment the next line and comment out the previous one
#CC=clang++
#LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++
# Assuming you want to use a recent compiler

# Compiler flags
CXXFLAGS= -g -Wall -std=c++11

all: ../bin/Matrix

../bin/Matrix: ../src/main.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/$(PROGRAM_NAME) $(LIBS)
	
clean:
	rm -rf *.o ../bin/$(PROGRAM_NAME)
