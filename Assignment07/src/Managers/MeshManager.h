/*
*	MeshManager
*
*		Mesh* load(name)
*			if mesh loaded, return mesh, otherwise load mesh and then return it
()
*		clear
*/
#include <string>
#include <map>

#include "Engine.h"

#include "GraphicsManager.h"
#include "TextureManager.h"

class MeshManager { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			Engine* engine;

			GraphicsManager* graphicsManager;
			TextureManager* textureManager;

			void onDestroy();
			//this calls destructor

/*		Constructors
*		----------------------------
*/
			MeshManager(Engine* engine);
			~MeshManager();
/*
*****************************************************************
*
*/	public: /*
*
*		Operations
*		----------------------------------
*/
			Mesh* loadMesh(const std::string& filename);
			void unload(const std::string& filename);
			void unload(const Mesh* mesh);

			Event<void> eventDestroy;

			void clear();
/*
*****************************************************************/
}; //MeshManager