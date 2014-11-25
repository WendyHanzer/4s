#include "ComponentContext.h"

ComponentContext::ComponentContext() : idCount(1) { }

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
		types[type] = new ComponentContext::Mapping(idCount++, type.name(), system);
	} //if

	return ((Mapping&)(*types[type]));
} //create

ComponentContext::Mapping& ComponentContext::operator[](const std::type_index& type)
{
	auto it = types.find(type);

	if (it != types.end())
	{
		return *it->second;
	} //if

	return ComponentContext::Mapping::null;
} //get

ComponentContext::Mapping ComponentContext::Mapping::null(0, "INVALID");
