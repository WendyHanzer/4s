#include "TypeManager.h"

ComponentContext::ComponentContext() : idCount(0) { }

ComponentContext::~ComponentContext()
{
    for (auto& it : types)
    {
        delete it.second;
    } //for
} //~TypeManager

ComponentContext::Mapping& ComponentContext::add(System& system)
{
	auto& type = typeid(system);

	if (types.find(type) == types.end())
	{
		types[type] = new TypeManager::Type(++idCount, type.name(), system);
	} //if

	return *types[type];
} //create

ComponentContext::Mapping& ComponentContext::operator[](const std::type_index& type)
{
	auto it = types.find(type);

	if (it != types.end())
	{
		return *it->second;
	} //if

	return TypeManager::Type::null;
} //get

const ComponentContext::Mapping ComponentContext::Mapping::null(-1, "INVALID");
