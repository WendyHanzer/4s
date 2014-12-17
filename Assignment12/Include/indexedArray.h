#pragma once

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>

#include <string>

template<typename T>
class IndexedArray
{
private:
	int capacity;
	int size;
	T* arr;

public:

	IndexedArray(int initialCapacity = 50)
	{
		if (initialCapacity <= 0)
		{
			initialCapacity = 50;
		} //if

		capacity = initialCapacity;
		size = 0;
		arr = new T[initialCapacity];
	} //IndexedArray

	~IndexedArray()
	{
		delete[] arr;
	} //~IndexedArray()

	void push(T& value)
	{
		if (size == capacity)
		{
			T* arrNew = new T[capacity << 1];
			memcpy(arrNew, arr, capacity*sizeof(T));
			T* arrOld = arr;
			arr = arrNew;
			delete[] arrOld;
		} //if

		value.copyTo(arr[size++]);
	} //push

	inline T& operator[](int i) { return arr[i]; }
	inline void clear() { size = 0; }
	inline const int& getSize() { return size; }
}; //IndexedArray