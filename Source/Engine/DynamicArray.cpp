#include "DynamicArray.h"
int DynamicArray::growArray(char *& array)
{
	const int size = sizeof(array) / sizeof(array[0]);
	int newSize = size + 1; // calc new size

	char* newArray = new char[newSize]; // create new array

	// copy old array to new array
	for (int i = 0; i < size; i++)
	{
		newArray[i] = array[i];
	}

	delete[] array; // get rid of the old one

	array = newArray;

	return newSize;
}
