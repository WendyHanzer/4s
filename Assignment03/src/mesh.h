#pragma once

#define GLM_FORCE_RADIANS

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>

#include "vertex.h"

#include "program.h"
#include "texture.h"

using namespace std;

/*
*	work in progress
*/	

#define INVALID_VALUE 0xFFFFFFFF
class Mesh
{
public:
	struct SubMesh
	{
		GLuint vertexBuffer;
		GLuint materialBuffer;
		unsigned int indexCount;
		unsigned int materialIndex;
		
		SubMesh()
		{
			vertexBuffer = INVALID_VALUE;
			materialBuffer = INVALID_VALUE;
			
			materialIndex = INVALID_VALUE;
			indexCount = 0;
		}
		
		void init(const Vertex* vertices, const GLuint* indices, const unsigned int& vertexCount, const unsigned int& faceCount)
		{
			indexCount = faceCount;
		
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexCount, vertices, GL_STATIC_DRAW);
			
			glGenBuffers(1, &materialBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, materialBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*faceCount, indices, GL_STATIC_DRAW);
		}

		~SubMesh()
		{
			if (vertexBuffer != INVALID_VALUE)
			{
				glDeleteBuffers(1, &vertexBuffer);
			} //if
			
			if (materialBuffer != INVALID_VALUE)
			{
				glDeleteBuffers(1, &materialBuffer);
			} //if
		}
	};

	SubMesh* subMeshes;
	Texture** textures;
	unsigned int subMeshCount;
	unsigned int materialCount;
	
	Mesh(const aiScene* scene, const char* filename)
	{
		std::cout << "Loading: " << filename << endl;
	
		subMeshCount = scene->mNumMeshes;
		
		if (subMeshCount)
		{
			subMeshes = new SubMesh[subMeshCount];
			
			for (unsigned int i = 0; i < subMeshCount; ++i)
			{
				std::cout << "Loading Sub Mesh: " << i << endl;
				loadSubMesh(i, scene->mMeshes[i]);
			}
		} //if
		
		materialCount = scene->mNumMaterials;
		
		std::cout << "Material Count: " << materialCount << endl;
		
		if (materialCount)
		{
			textures = new Texture*[materialCount];
			loadMaterials(scene, filename);
		} //if
	} //constructor
	
	void loadSubMesh(unsigned int index, const aiMesh* subMesh)
	{
		unsigned int vertexCount = subMesh->mNumVertices;
		unsigned int faceCount = subMesh->mNumFaces;
		
		Vertex* vertices = new Vertex[vertexCount];
		GLuint* indices = new GLuint[faceCount];
		
		subMeshes[index].materialIndex = subMesh->mMaterialIndex;
		
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			std::cout << "Creating Vertex: " << i << endl;
		
			if (subMesh->HasTextureCoords(0))
			{
				vertices[i].texture[0] = subMesh->mTextureCoords[0][i].x;
				vertices[i].texture[1] = subMesh->mTextureCoords[0][i].y;
			} //if
			else
			{
				vertices[i].texture[0] = 0;
				vertices[i].texture[1] = 1;
			} //else
			
			/*
			if (subMesh->HasVertexColors(0))
			{
				vertices[i].color[0] = subMesh->mColors[i]->r;
				vertices[i].color[1] = subMesh->mColors[i]->g;
				vertices[i].color[2] = subMesh->mColors[i]->b;
				vertices[i].color[3] = subMesh->mColors[i]->a;
			} //if
			else
			{
				vertices[i].color[0] = 0;
				vertices[i].color[1] = 0;
				vertices[i].color[2] = 0;
				vertices[i].color[3] = 0;
			} //else
			*/
			
			if (subMesh->HasNormals())
			{
				vertices[i].normal[0] = subMesh->mNormals[i].x;
				vertices[i].normal[1] = subMesh->mNormals[i].y;
				vertices[i].normal[2] = subMesh->mNormals[i].z;
			} //if
			else
			{
				vertices[i].normal[0] = 0;
				vertices[i].normal[1] = 0;
				vertices[i].normal[2] = 0;
			} //else
			
			vertices[i].position[0] = subMesh->mVertices[i].x;
			vertices[i].position[1] = subMesh->mVertices[i].y;
			vertices[i].position[2] = subMesh->mVertices[i].z;
			
		} //for
		
		for (unsigned int i = 0; i < subMesh->mNumFaces; ++i)
		{
			indices[i*3] = subMesh->mFaces[i].mIndices[0];
			indices[i*3 + 1] = subMesh->mFaces[i].mIndices[1];
			indices[i*3 + 2] = subMesh->mFaces[i].mIndices[2];
		} //for

		subMeshes[index].init(vertices, indices, vertexCount, faceCount);
		
		//delete[] vertices;
		//delete[] indices;
	}
	
	void loadMaterials(const aiScene* scene, const char* filename)
	{
		int lastSlash;
		std::string directory;
		std::string fullPath;
		aiString path;
		
		cout << " Loading material: " << filename << endl;
		
		/*
		*	get directory out of filename
		*/
		for (lastSlash = sizeof(filename)/sizeof(char); lastSlash >= 0 && filename[lastSlash] != '/'; --lastSlash) {}
		
		if (lastSlash == -1)
		{
			directory = ".";
		} //if
		else if (lastSlash == 0)
		{
			directory = "/";
		} //else if
		else
		{
			directory = std::string(filename).substr(0, lastSlash);
		} //else

		/*
		*	initialize materials
		*/
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			textures[i] = NULL;
			
			if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					fullPath = directory + path.data;
					textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

					if (textures[i]->image == NULL)
					{
						std::cerr << "[F] TEXTURE LOAD ERROR: " << fullPath.c_str() << std::endl;
						delete textures[i];
						textures[i] = NULL;
					} //if
					else
					{
						std::cout << "Loaded " << filename << std::endl;
					} //else
				} //if
			} //if
			
			if (!textures[i])
			{
				textures[i] = new Texture(GL_TEXTURE_2D, "models/borg.jpg");
			}
		}
	}
	
	void render(const Program& program)
	{
		//glValidateProgram(program.program);
		glUseProgram(program.program);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		//glEnableVertexAttribArray(program.location.position);
		//glEnableVertexAttribArray(program.location.color);

		for (unsigned int i = 0; i < subMeshCount; ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, subMeshes[i].vertexBuffer);
			
			glVertexAttribPointer(	0,										//location of attribute
									3,										//number of elements
									GL_FLOAT,								//type
									GL_FALSE,								//normalized?
									sizeof(Vertex),							//stride
									(void*)offsetof(Vertex,position));		//offset

			glVertexAttribPointer(	1,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vertex),
									(void*)offsetof(Vertex,texture));
									
			glVertexAttribPointer(	2,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vertex),
									(void*)offsetof(Vertex,normal));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMeshes[i].materialBuffer);

			if (subMeshes[i].materialIndex < materialCount && textures[subMeshes[i].materialIndex])
			{
				textures[subMeshes[i].materialIndex]->bind(GL_TEXTURE0);
			}

			glDrawElements(GL_TRIANGLES, subMeshes[i].indexCount, GL_UNSIGNED_INT, 0);
		}
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glUseProgram(0);
	}
	
	~Mesh()
	{
		if (subMeshCount)
		{
			delete[] subMeshes;
		} //if
		
		for (unsigned int i = 0; i < materialCount; ++i)
		{
			delete textures[i];
		} //for
		
		if (materialCount)
		{
			delete[] textures;
		} //if
	} //destructor
}; //Mesh

Mesh* loadMesh(const char* filename)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (!scene)
	{
		std::cerr << "[F] MESH LOAD ERROR " << filename << ": " << importer.GetErrorString() << std::endl;
		
		return 0;
	}
	
	return new Mesh(scene, filename);
}
