#pragma once

#include <unordered_map>

#include "../../Utils/TupleHash.h"
#include "../Behavior/DestructBehavior.h"

/*
*	Manager<Type, Args...>
*
*		Constructs a manager that manages values of type Value
*		under a set of arguments. Will only ever create a value
*		once, then just returns the created value.
*
*		Values are automatically destroyed when the manager is
*		destroyed.
*
*		Values must throw exceptions when failing to load. For
*		example, if a file was not found for a texture, it should
*		throw an exception.
*
*		Manager(System& system)
*		~Manager()
*
*		Type* ()(arg1, arg2, arg3, ...)
*		void unload(arg1, arg2, arg3, ...)
*		void clear()
*
**********************************************************************
*
*	Example
*
*		Manager<string, int, int, string> myManager;
*		string* s = myManager(1, 2, "hello");
*		cout << *s << endl; //"hello"
*/

template <class V, class... Keys>
class Manager : public DestructBehavior<>
{
	private:
	
		const Manager& operator=(const Manager& manager) { return *this; }
		
		std::unordered_map<std::tuple<Keys...>, V*> objects;
		
	public:
	
		Manager(DestructBehaviorEvent& behavior) : DestructBehavior(behavior) { }
		
		virtual ~Manager()
		{
			destroy();
		
			clear();
		} //~Manager

		V* operator()(Keys&&... args)
		{
			const std::tuple<Keys...> key(args...);
		
			auto it = objects.find(key);
			V* object = NULL;

			if (it == objects.end())
			{
				try
				{
					object = new V(args...);
				} //try
				catch (int e)
				{
					object = NULL;
				} //catch

				if (object)
				{
					objects[key] = object;
				} //if
			} //if
			else
			{
				object = it->second;
			} //else

			return object;
		} //load
		
		void unload(Keys&&... args)
		{
			const std::tuple<Keys...> key(args...);
			auto it = objects.find(key);

			if (it != objects.end())
			{
				delete it->second();
				objects.erase(key);
			} //if
		} //unload
		
		void clear()
		{
			for (auto& it : objects)
			{
				delete it.second;
			} //for
		} //clear
}; //Manager


