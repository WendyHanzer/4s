#pragma once

/*
*	IdManager<T>
*
*		IdManager()
*
*		T& IdManager[id]
*
*		unsigned int allocate(const T& object)
*		void deallocate(const unsigned int& id)
*/

#include <vector>
#include <stack>

template <class T>
class IdManager
{
	private:
			/*
			*	Allocator
			*/
			unsigned int idCount;
			std::stack<unsigned int> idRecycler;

			/*
			*	Members
			*/
			std::vector<T*> objects;

	public:
			IdManager() : idCount(0)
            {
                objects.resize(2);
            } //IdManager

            inline T& operator[](const unsigned int& id)
			{
				return *objects[id];
			} //get

			unsigned int allocate(T& object)
			{
				unsigned int id;

				if (idRecycler.empty())
				{
					id = ++idCount;
				} //if
				else
				{
					id = idRecycler.top();
					idRecycler.pop();
				} //else

				if (id >= objects.size())
				{
					objects.resize(objects.size() + 1);
				} //if

				objects[id] = &object;

                return id;
			} //allocate

			void deallocate(const unsigned int& id)
			{
				if (id < objects.size() && objects[id])
				{
					idRecycler.push(id);
					objects[id] = NULL;
				} //if
			} //deallocate
}; //IdManager
