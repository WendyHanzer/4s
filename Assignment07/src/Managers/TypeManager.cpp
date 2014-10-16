#include "TypeManager.h"

TypeManager::TypeManager(Engine* engine, std::type_info type) : Manager(engine, type)
{
	idCount = 0;
} //TypeManager

TypeManager::Type* TypeManager::create(std::type_info type)
{
	types[type] = {++idCount, type.name()};
	return &types[type];
} //create

TypeManager::Type* TypeManager::get(std::type_info type)
{
	auto it = types.find();

	if (it != types.end())
	{
		return &(it->second);
	}

	return NULL;
} //get