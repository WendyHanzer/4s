#include "TextureManager.h"

TextureManager::TextureManager(Engine* engine) : Manager(engine, typeid(TextureManager))
{

} //TextureManager

TextureManager::~TextureManager()
{
    for (auto it : textures)
    {
        delete it->second;
    } //for
} //~TextureManager

Texture* TextureManager::load(GLenum textureTarget, const std::string& filename)
{
    auto it = textures.find(filename);
    Texture* texture;

    if (it == textures.end())
    {
        texture = new Texture(textureTarget, filename);
        textures[filename] = texture;
    } //if
    else
    {
        texture = it->second();
    } //else

    return texture;
} //load

void unload(const std::string& filename)
{
    auto it = textures.find(filename);

    if (it != textures.end())
    {
        delete it->second();
        textures.erase(filename);
    } //if
} //unload