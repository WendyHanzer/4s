#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

#include "Manager.h"
#include "../Graphics/Texture.h"
#include "../Utils/Event.h"

class Texture;

class TextureManager : public Manager { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			std::unordered_map<std::string, Texture*> textures;
/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------------
*/
			TextureManager(Engine* engine);
			virtual ~TextureManager();
/*
*		Methods
*		----------------------------------
*/
            Texture* load(GLenum textureTarget, const std::string& filename);
			void unload(const std::string& filename);
/*
*****************************************************************/
}; //TextureManager
