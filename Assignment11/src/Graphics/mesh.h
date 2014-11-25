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

#include "shaderProgram.h"
#include "texture.h"

#include <fstream>

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
		GLuint VBO;
		GLuint IBO;
		GLuint materialIndex;
		GLuint indexCount;
		
		glm::vec3 specularColor;
		glm::vec3 diffuseColor;
		glm::vec3 ambientColor;
		float shine;

		SubMesh()
		{
			VBO = INVALID_VALUE;
			IBO = INVALID_VALUE;

			materialIndex = INVALID_VALUE;

			indexCount = 0;
		}

		void init(const vector<Vertex>& vertices, const vector<GLuint>& indices)
		{
			this->indexCount = indices.size();
			
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.capacity(), vertices.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.capacity(), indices.data(), GL_STATIC_DRAW);
		}

		~SubMesh()
		{
			if (VBO != INVALID_VALUE)
			{
				glDeleteBuffers(1, &VBO);
			} //if

			if (IBO != INVALID_VALUE)
			{
				glDeleteBuffers(1, &IBO);
			} //if
		}
	};

	vector<SubMesh> subMeshes;
	vector<Texture*> textures;

	vector<GLfloat> vertices;
	vector<GLint> indices;
	
	Texture* texture;
	
	Mesh(const aiScene* scene, const char* filename) : texture(NULL)
	{
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;

		if (scene->mNumMeshes)
		{
			subMeshes.resize(scene->mNumMeshes);

			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				vertexCount += scene->mMeshes[i]->mNumVertices*3;
				indexCount += scene->mMeshes[i]->mNumFaces*3;
			} //for
			
			this->vertices.reserve(vertexCount);
			this->indices.reserve(indexCount);

			this->vertices.resize(vertexCount);
			this->indices.resize(indexCount);

			vertexCount = 0;
			indexCount = 0;
			
			for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			{
				loadSubMesh(i, scene->mMeshes[i], vertexCount, indexCount);

				vertexCount += scene->mMeshes[i]->mNumVertices*3;
				indexCount += scene->mMeshes[i]->mNumFaces*3;
			}
		} //if

		if (scene->mNumMaterials)
		{
			textures.resize(scene->mNumMaterials);

			loadMaterials(scene, filename);
		} //if
	} //constructor

	void loadSubMesh(unsigned int index, const aiMesh* subMesh, const unsigned int& startVertex, const unsigned int& startIndex)
	{
		vector<Vertex> verticesTemp;
		vector<GLuint> indicesTemp;

		verticesTemp.resize(subMesh->mNumVertices);
		indicesTemp.reserve(subMesh->mNumFaces*3);

		subMeshes[index].materialIndex = subMesh->mMaterialIndex;

		/*
		*   load vertices
		*/
		for (unsigned int i = 0; i < subMesh->mNumVertices; ++i)
		{
			if (subMesh->HasTextureCoords(0))
			{
				verticesTemp[i].texture[0] = subMesh->mTextureCoords[0][i].x;
				verticesTemp[i].texture[1] = subMesh->mTextureCoords[0][i].y;
			} //if
			else
			{
				verticesTemp[i].texture[0] = 0;
				verticesTemp[i].texture[1] = 1;
			} //else

			/*
			if (subMesh->HasVertexColors(0))
			{
				verticesTemp[i].color[0] = subMesh->mColors[i]->r;
				verticesTemp[i].color[1] = subMesh->mColors[i]->g;
				verticesTemp[i].color[2] = subMesh->mColors[i]->b;
				verticesTemp[i].color[3] = subMesh->mColors[i]->a;
			} //if
			else
			{
				verticesTemp[i].color[0] = 0;
				verticesTemp[i].color[1] = 0;
				verticesTemp[i].color[2] = 0;
				verticesTemp[i].color[3] = 0;
			} //else
			*/

			if (subMesh->HasNormals())
			{
				verticesTemp[i].normal[0] = subMesh->mNormals[i].x;
				verticesTemp[i].normal[1] = subMesh->mNormals[i].y;
				verticesTemp[i].normal[2] = subMesh->mNormals[i].z;
			} //if
			else
			{
				verticesTemp[i].normal[0] = 0;
				verticesTemp[i].normal[1] = 0;
				verticesTemp[i].normal[2] = 0;
			} //else
			
			//COLOR_SPECULAR

			verticesTemp[i].position[0] = subMesh->mVertices[i].x;
			verticesTemp[i].position[1] = subMesh->mVertices[i].y;
			verticesTemp[i].position[2] = subMesh->mVertices[i].z;

			vertices[startVertex + (i*3 + 0)] = verticesTemp[i].position[0];
			vertices[startVertex + (i*3 + 1)] = verticesTemp[i].position[1];
			vertices[startVertex + (i*3 + 2)] = verticesTemp[i].position[2];
		} //for

		/*
		*   load faces
		*/
		for (unsigned int i = 0; i < subMesh->mNumFaces; ++i)
		{
			indicesTemp.push_back(subMesh->mFaces[i].mIndices[0]);
			indicesTemp.push_back(subMesh->mFaces[i].mIndices[1]);
			indicesTemp.push_back(subMesh->mFaces[i].mIndices[2]);
			
			indices[startIndex + (i*3 + 0)] = indicesTemp[i*3 + 0]*3 + startVertex;
			indices[startIndex + (i*3 + 1)] = indicesTemp[i*3 + 1]*3 + startVertex;
			indices[startIndex + (i*3 + 2)] = indicesTemp[i*3 + 2]*3 + startVertex;
		} //for
		
		subMeshes[index].init(verticesTemp, indicesTemp);
	}

	void loadMaterials(const aiScene* scene, const char* filename)
	{
		int lastSlash;
		string directory;
		string fullPath;
		aiString path;

		/*
		*   get directory out of filename
		*/
		for (lastSlash = sizeof(filename)/sizeof(char) - 1; lastSlash >= 0 && filename[lastSlash] != '/'; --lastSlash) {}

		if (lastSlash == -1)
		{
			directory = "./";
		} //if
		else if (lastSlash == 0)
		{
			directory = "/";
		} //else if
		else
		{
			directory = string(filename).substr(0, lastSlash) + "/";
		} //else

		/*
		*   initialize materials
		*/
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			textures[i] = NULL;

			if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					fullPath = directory + path.data;
					cout << "opening " << fullPath << endl;
					textures[i] = new Texture(fullPath.c_str());

					if (!textures[i]->loaded)
					{
						std::cerr << "[F] TEXTURE LOAD ERROR: " << fullPath.c_str() << std::endl;

						delete textures[i];
						textures[i] = NULL;
					} //if
					else
					{
						cout << "loaded material " << i << endl;
					} //else
				} //if
			} //if
		} //for
	}

	void render(ShaderProgram& program, Texture* super = NULL)
	{
		GLuint position = program.attribute("position");
		GLuint uv = program.attribute("uv");
		GLuint normal = program.attribute("normal");
		GLuint sampler = program.uniform("sampler");
		GLuint specular = program.uniform("materialSpecular");
		GLuint diffuse = program.uniform("materialDiffuse");
		GLuint ambient = program.uniform("materialAmbient");
		GLuint shine = program.uniform("materialShine");
		
		glUniform1i(sampler, 0);

		glEnableVertexAttribArray(position);
		glEnableVertexAttribArray(uv);
		glEnableVertexAttribArray(normal);

		bool hasTexture;
		bool globalTexture = texture != NULL && texture->loaded;

		for (unsigned int i = 0; i < subMeshes.capacity(); ++i)
		{
			/*
			*	because none of the materials currently use shininess, specular, diffuse, etc
			*	just set factors to 1 and shine to 20
			*/
			glUniform4f(specular, 1, 1, 1, 1);
			glUniform4f(diffuse, 1, 1, 1, 1);
			glUniform4f(ambient, 1, 1, 1, 1);
			glUniform1f(shine, 3);
		
			glBindBuffer(GL_ARRAY_BUFFER, subMeshes[i].VBO);

			glVertexAttribPointer(position, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
			glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));
			glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMeshes[i].IBO);

			//should set it per inner mesh
			if (super)
			{
				super->bind(GL_TEXTURE0);
			} //if
			else if (globalTexture)
			{
				this->texture->bind(GL_TEXTURE0);
			} //if
			else
			{
				hasTexture = subMeshes[i].materialIndex < textures.capacity() && textures[subMeshes[i].materialIndex];
				if (hasTexture)
				{
					textures[subMeshes[i].materialIndex]->bind(GL_TEXTURE0);
				} //if
			} //else

			glDrawElements(GL_TRIANGLES, subMeshes[i].indexCount, GL_UNSIGNED_INT, 0);
			
			if (super)
			{
				super->unbind(GL_TEXTURE0);
			} //if
			else if (globalTexture)
			{
				this->texture->unbind(GL_TEXTURE0);
			} //if
			else if (hasTexture)
			{
				textures[subMeshes[i].materialIndex]->unbind(GL_TEXTURE0);
			} //else if
		}

		glDisableVertexAttribArray(position);
		glDisableVertexAttribArray(uv);
		glDisableVertexAttribArray(normal);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~Mesh()
	{
		for (Texture* texture : textures)
		{
			if (texture)
			{
				delete texture;
			} //if
		} //for
	} //Mesh
}; //Mesh

Mesh* loadMesh(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);

	if (!scene)
	{
		std::cerr << "[F] MESH LOAD ERROR " << filename << ": " << importer.GetErrorString() << std::endl;

		return 0;
	}

	Mesh* mesh = new Mesh(scene, filename);

	//delete scene;

	return mesh;
}
