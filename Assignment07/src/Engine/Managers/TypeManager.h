#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>

/*
*	TypeManager
*
*		TypeManager::Type
*
*			(string)Type
*				-	gives name
*
*			(int)Type
*				-	gives id
*
*			Type1 == Type2
*				-	compares id
*
*			inline bool isNull()
*				-	id == -1?
*
*		TypeManager()
*
*		TypeManager::Type& create(const std::type_index& type)
*			-	creates new type
*
*		Type& Type[typeid(class)]
*			-	retrieves the type of a class/struct or returns null type
*
*		(int)TypeManager
*			-	gives count
*
*			-	int values[(int)TypeManager]
*/

class TypeManager final
{
    public:

        struct Type;

	private:

		std::unordered_map<std::type_index, Type*> types;
		unsigned int idCount;

	public:
		struct Type final
		{
            private:

                const unsigned int id;
                const std::string name;

            public:

                static Type null;

                Type(const unsigned int& id, const std::string& name) : id(id), name(name) { }

                inline bool operator==(const Type& other) const { return id == other.id; }
                inline bool isNull() { return id == null.id; }

                inline operator const std::string& () { return name; }
                inline operator const unsigned int& () { return id; }
		}; //Type

		TypeManager();
        ~TypeManager();

		Type& create(const std::type_index& type);
        Type& operator[](const std::type_index& type);

		inline operator const unsigned int& () { return idCount; }
}; //TypeManager
