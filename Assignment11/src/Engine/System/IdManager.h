#pragma once

/*
*	IdManager
*
*		IdManager()
*
*		const unsigned int allocate()
*		void deallocate(const unsigned int& id)
*/

#include <stack>

class IdManager final
{
	private:
			/*
			*	Allocator
			*/
			unsigned int idCount;
			std::stack<unsigned int> idRecycler;

	public:
			IdManager() : idCount(0) { }

			const unsigned int allocate()
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

                return id;
			} //allocate

			inline void deallocate(const unsigned int& id)
			{
				idRecycler.push(id);
			} //deallocate
}; //IdManager
