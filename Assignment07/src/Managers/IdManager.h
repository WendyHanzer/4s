#pragma once

/*
*	IdManager<T>
*
*		Event<void> eventDestroy
*
*		IdManager(Engine* engine)
*		~IdManager()
*
*		inline T get(const int& id)
*
*		int allocate(T object)
*		void deallocate(const int& id)
*/

#include <vector>
#include <stack>

#include "Manager.h"

template <class T>
class IdManager : public Manager
{
	private:
			/*
			*	Allocator
			*/
			int idCount;
			std::stack<int> idRecycler;

			/*
			*	Members
			*/
			std::vector<T> objects;

	public:
			IdManager(Engine* engine, std::type_info type) : Manager(engine, type)
			{
				idCount = 0;
			} //IdManager

			inline T get(const int& id)
			{
				return objects[id];
			} //get

			int allocate(T object)
			{
				int id;

				if (idRecycler.empty())
				{
					id = idRecycler.top();
					idRecycler.pop();
				} //if
				else
				{
					id = ++idCount;
				} //else

				if (id >= objects.capacity())
				{
					objects.reserve(objects.capacity() << 1);
				} //if

				objects[id] = object;
			} //allocate

			void deallocate(const int& id)
			{
				if (id < objects.capacity() && objects[id])
				{
					idRecycler.push(id);
					objects[id] = NULL;
				} //if
			} //deallocate
}; //IdManager