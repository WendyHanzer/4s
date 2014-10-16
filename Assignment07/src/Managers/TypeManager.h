#pragma once

#include <unordered_map>
#include <typeinfo>

#include "Manager.h"
#include "../Engine.h"

class Engine;
class Manager;

/*
*	Aspect types
*	Manager types
*
*	Used to allow managers, aspects, and so forth to be stored in arrays or vectors instead of maps
*/

class TypeManager : public Manager
{
	private:

		std::unordered_map<std::type_info, Type> types;
		int idCount;

	public:
		struct Type
		{
			const int id;
			const std::string name;
		}; //Type

		TypeManager(Engine* engine, std::type_info type);

		Type* create(std::type_info type);
		Type* get(std::type_info type);

		inline void count() { return idCount; }
}; //TypeManager