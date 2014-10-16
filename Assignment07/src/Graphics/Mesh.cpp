#include "Mesh.h"

#define GLM_FORCE_RADIANS

#define INVALID_VALUE 0xFFFFFFFF

Mesh::SubMesh::SubMesh()
{
	VBO = INVALID_VALUE;
	IBO = INVALID_VALUE;

	materialIndex = INVALID_VALUE;

	indexCount = 0;
}

void Mesh::SubMesh::init(const vector<Vertex>& vertices, const vector<GLuint>& indices)
{
	this->indexCount = indices.size();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.capacity(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.capacity(), indices.data(), GL_STATIC_DRAW);
}

Mesh::SubMesh::~SubMesh()
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


Mesh::Mesh(const aiScene* scene, const char* filename)
{
	if (scene->mNumMeshes)
	{
		Mesh::subMeshes.resize(scene->mNumMeshes);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			loadSubMesh(i, scene->mMeshes[i]);
		}
	} //if

	if (scene->mNumMaterials)
	{
		Mesh::textures.resize(scene->mNumMaterials);

		loadMaterials(scene, filename);
	} //if
} //constructor

void Mesh::loadSubMesh(unsigned int index, const aiMesh* subMesh)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;

	vertices.resize(subMesh->mNumVertices);
	indices.reserve(subMesh->mNumFaces*3);

	Mesh::subMeshes[index].materialIndex = subMesh->mMaterialIndex;

	/*
	*   load vertices
	*/
	for (unsigned int i = 0; i < subMesh->mNumVertices; ++i)
	{
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

	/*
	*   load faces
	*/
	for (unsigned int i = 0; i < subMesh->mNumFaces; ++i)
	{
		indices.push_back(subMesh->mFaces[i].mIndices[0]);
		indices.push_back(subMesh->mFaces[i].mIndices[1]);
		indices.push_back(subMesh->mFaces[i].mIndices[2]);
	} //for

	subMeshes[index].init(vertices, indices);
}

void Mesh::loadMaterials(const aiScene* scene, const char* filename)
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
		Mesh::textures[i] = NULL;

		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				fullPath = directory + path.data;
				cout << "opening " << fullPath << endl;
				Mesh::textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

				if (!Mesh::textures[i]->loaded)
				{
					std::cerr << "[F] TEXTURE LOAD ERROR: " << fullPath.c_str() << std::endl;

					delete Mesh::textures[i];
					Mesh::textures[i] = NULL;
				} //if
				else
				{
					cout << "loaded material " << i << endl;
				} //else
			} //if
		} //if

		if (!Mesh::textures[i])
		{
			//textures[i] = new Texture(GL_TEXTURE_2D, "models/borg.jpg");
		}
	}
}

void Mesh::render(ShaderProgram& program, const glm::mat4 mvpMatrix)
{
	//glValidateProgram(program.program);
	program.use();

	GLuint position = program.attribute("vertexPosition_modelspace");
	GLuint uv = program.attribute("vertexUV");
	GLuint sampler = program.uniform("sampler");
	//GLuint normal = program.attribute("vertexNormal_modelspace");

	GLuint mvp = program.uniform("MVP");

	glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniform1i(sampler, 0);

	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(uv);
	//glEnableVertexAttribArray(normal);

	bool hasTexture;

	for (unsigned int i = 0; i < Mesh::subMeshes.capacity(); ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, Mesh::subMeshes[i].VBO);

		glVertexAttribPointer(position, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
		glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));
		//glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh::subMeshes[i].IBO);

		hasTexture = Mesh::subMeshes[i].materialIndex < Mesh::textures.capacity() && Mesh::textures[Mesh::subMeshes[i].materialIndex];
		if (hasTexture)
		{
			Mesh::textures[Mesh::subMeshes[i].materialIndex]->bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, Mesh::subMeshes[i].indexCount, GL_UNSIGNED_INT, 0);

		if (hasTexture)
		{
			Mesh::textures[Mesh::subMeshes[i].materialIndex]->unbind(GL_TEXTURE0);
		}
	}

	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(uv);
	//glDisableVertexAttribArray(normal);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	program.disable();
}

Mesh::~Mesh()
{
	for (Texture* texture : Mesh::textures)
	{
		if (texture)
		{
			delete texture;
		} //if
	} //for
} //Mesh

