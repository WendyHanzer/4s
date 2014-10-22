#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>

#include "../../System/System.h"

/*
*	ComponentContext
*
*		ComponentContext::Mapping
*
*			(string)Mapping
*				-	gives name
*
*			(int)Mapping
*				-	gives id
*
*			(System)Mapping
*				-	gives system
*
*			Controller1 == Controller2
*				-	compares id
*
*			inline bool isNull()
*				-	id == -1?
*
*		ComponentContext()
*
*		ComponentContext::Mapping& add(const std::type_index& type)
*			-	creates new type
*
*		ComponentContext::Mapping& ComponentContext[typeid(class)]
*			-	retrieves the type of a class/struct or returns null type
*
*		(int)ComponentContext
*			-	gives count
*
*			-	int values[(int)TypeManager]
*/

class ComponentContext final
{
    public:

        struct Mapping;

	private:
	
		std::unordered_map<std::type_index, Mapping*> types;
		unsigned int idCount;
		
		const ComponentContext& operator=(const ComponentContext& componentContext) { return *this; }

	public:
		struct Mapping final
		{
            private:

				const unsigned int id;
				const std::string name;
				
				System* system;		//root system, such as Graphics
				
				Mapping(const unsigned int& id, const std::string& name) : id(id), name(name), system(NULL) { }

            public:
            
				static const Mapping null;
				
				Mapping(const unsigned int& id, const std::string& name, System& system) : id(id), name(name), system(&system) { }

				inline const bool operator==(const Controller& other) const { return id == other.id; }
				inline const bool isNull() { return system; }

				inline operator const std::string& () { return name; }
				inline operator const unsigned int& () { return id; }
				inline operator System& () { return *system; }
		}; //Controller

		ComponentContext();
		~ComponentContext();

		Mapping& add(System& system);
		Mapping& operator[](const std::type_index& type);

		inline operator const unsigned int& () { return idCount; }
}; //TypeManager
