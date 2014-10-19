#include "TypeManager.h"

TypeManager::TypeManager() : idCount(0) { }

TypeManager::~TypeManager()
{
    for (auto& it : types)
    {
        delete it.second;
    } //for
} //~TypeManager

TypeManager::Type& TypeManager::create(const std::type_index& type)
{
	if (types.find(type) == types.end())
	{
		types[type] = new TypeManager::Type(++idCount, type.name());
	} //if

	return *types[type];
} //create

TypeManager::Type& TypeManager::operator[](const std::type_index& type)
{
	auto it = types.find(type);

	if (it != types.end())
	{
		return *it->second;
	} //if

	return TypeManager::Type::null;
} //get

TypeManager::Type TypeManager::Type::null(-1, "INVALID");