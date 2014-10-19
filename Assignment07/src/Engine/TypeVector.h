#pragma once

#include <vector>

#include "Managers/TypeManager.h"

template <class T>
class TypeVector
{
	private:

		std::vector<T*> objects;
		TypeManager types;

	public:

		TypeVector()
		{
			objects.resize(2);
		} //TypeVector

		TypeManager::Type& add(T& object, const std::type_index& objectType)
		{
			TypeManager::Type& type = types.create(objectType);
			
			if (type >= objects.size())
			{
				objects.resize(objects.size() + 1);
			} //if

			objects[type] = &object;

			return type;
		} //add
		
		inline TypeManager::Type& getType(const std::type_index& type)
		{
			return types[type];
		} //getType

		inline T& operator[](const std::type_index& type)
		{
			return *objects[(const unsigned int&)getType(type)];
		} //[]
		
		inline operator const unsigned int& () { return (const unsigned int&)types; }
		
		inline const std::vector<T*>& getObjects() { return objects; }
}; //TypeVector
