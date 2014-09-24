#pragma once

#define GLM_FORCE_RADIANS

#include <fstream>
#include <glm/glm.hpp>
#include <cstdlib>
#include <cstring>
#include <vector>		//hate resizing arrays, but whatever
#include "vertex.h"

struct Obj
{
	std::vector<Vertex> vertices;
	
	bool valid;

	Obj(const char* filename)
	{
		std::vector<Vertex>& vertices = *new std::vector<Vertex>();
		Vertex vertex;
		
		vertex.position[0] = 0;
		
		std::vector<unsigned int> vertexIndices;
		
		std::vector<glm::vec3> temp_vertices; 

		char buffer[512];	//large to be able to take an entire line, no line > 512 bytes
		char* slash = NULL;
		unsigned int index = 0;
		
		ifstream file(filename);
	
		valid = file.is_open();
		if (!valid)
		{
			std::cerr << "[F] FILE COULD NOT BE OPENED: ";
			std::cerr << filename << std::endl;
	
			return;
		} //if
		
		unsigned int line = 0;
		while (file.good())
		{
			++line;
			
			file >> buffer;
			
			/*
			*	this will only get vertices and faces from file
			*
			*	ignore everything else
			*/
			switch(buffer[0])
			{
				case 'v':
					switch (buffer[1])
					{
						case 0:
							file >> vertex.position[0] >> vertex.position[1] >> vertex.position[2];
							vertex.color[0] = 0;
							vertex.color[1] = 0;
							vertex.color[2] = 0;
							vertex.color[3] = 1;
							vertices.push_back(vertex);
							
							break;
						case 'n':
							file.getline(buffer, 512, '\n');
							
							break;
						case 't':
							file.getline(buffer, 512, '\n');
							
							break;
						case 'p':
							file.getline(buffer, 512, '\n');
							
							break;
						default:
							std::cerr << "[F] " << filename << ": LINE (" << line << ") IGNORED DUE TO ERROR" << std::endl;
							file.getline(buffer, 512, '\n');
							
							break;
					}
					
					break;
				case 'f':
					file >> buffer;
					slash = strchr(buffer, '/');
					
					if (slash == NULL)
					{
						this->vertices.push_back(vertices[atoi(buffer) - 1]);
						
						file >> index;
						this->vertices.push_back(vertices[index - 1]);
						
						file >> index;
						this->vertices.push_back(vertices[index - 1]);
					} //if
					else
					{
						*slash = 0;
						this->vertices.push_back(vertices[atoi(buffer) - 1]);
						
						file >> buffer;
						*strchr(buffer, '/') = 0;
						this->vertices.push_back(vertices[atoi(buffer) - 1]);
						
						file >> buffer;
						*strchr(buffer, '/') = 0;
						this->vertices.push_back(vertices[atoi(buffer) - 1]);
					} //else
					
					break;
				case '#':
					file.getline(buffer, 512, '\n');
					
					break;
				default:
					std::cerr << "[F] " << filename << ": LINE (" << line << ") IGNORED DUE TO ERROR" << std::endl;
					file.getline(buffer, 512, '\n');
					
					break;
			} //swtich
		} //while
		
		file.close();
		
		delete &vertices;
	} //Obj
}; //Obj
