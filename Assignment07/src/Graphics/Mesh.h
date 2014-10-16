#pragma once

#define GLM_FORCE_RADIANS

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Vertex.h"

#include "ShaderProgram.h"
#include "Texture.h"

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
		/*
		*	Constructors
		*/
		SubMesh();
		~SubMesh();

		void init(const vector<Vertex>& vertices, const vector<GLuint>& indices);
	}; //SubMesh
	/*
	*	Vectors
	*/
	vector<SubMesh> subMeshes;
	vector<Texture*> textures;
	/*
	*	Constructors
	*/
	Mesh(const aiScene* scene, const char* filename);
	~Mesh();
	/*
	*	Load Operations
	*/
	void loadSubMesh(unsigned int index, const aiMesh* subMesh);
	void loadMaterials(const aiScene* scene, const char* filename);
	/*
	*	Render Operation
	*/
	void render(ShaderProgram& program, const glm::mat4 mvpMatrix);
}; //Mesh

Mesh* loadMesh(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		std::cerr << "[F] MESH LOAD ERROR " << filename << ": " << importer.GetErrorString() << std::endl;

		return 0;
	}

	Mesh* mesh = new Mesh(scene, filename);

	//delete scene;

	return mesh;
}
