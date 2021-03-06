#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include "Source/Engine/GameObject.h"
#include "Source/Engine/SmartPointer.h"
#include "Source/Engine/PhysicsComponent.h"

/*
* A unit test to demonstrate smart pointers on game objects
*/
int main()
{

	SmartPointer<GameObject> * master = new SmartPointer<GameObject>(new GameObject()); //master smart pointer 

	int count = 1024*1024;
	printf("Testing Smart Pointers: \n\n");

	/*
	* Smart pointer should provide accurate reference counting, 
	* releasing the underlying object when reference count reaches 0.
	*/
	std::vector<SmartPointer<GameObject>> pointers;

	for (int i = 0; i < count; i++) {

		pointers.push_back(SmartPointer<GameObject>(*master));
	}

	assert(master->getRefCounts().strongCount == count + 1);
	printf("assert(master->getRefCount() == count + 1)\n");

	for (int i = 0; i < count; i++) {
		pointers[i] = NULL; //destroy all clones
	}

	assert(master->getRefCounts().strongCount == 1);
	printf("assert(master->getRefCount() == 1)\n");

	//now release the underlying object
	*master = NULL;

	//make sure the underlying reference is now null
	assert(master->operator->() == nullptr);
	printf("assert((master->operator->()) == nullptr)\n\n");

	/*
	* Smart pointer should provide natural semantic access to the 
	* underlying pointer without providing direct access to the pointer.
	*/
	master = new SmartPointer<GameObject>(new GameObject("Old Name"));

	const char * tag = "New Name";
	const float mass = 10.0f;
	(*master)->tag = tag;
	(*master)->physics->mass = mass;

	assert((*master)->tag == tag);
	assert((*master)->physics->mass == mass);
	printf("assert((*master)->tag == tag)\n");
	printf("assert((*master)->physics.mass == mass)\n\n");

	/*
	* Should provide comparison operators. 
	*/
	SmartPointer<GameObject> copyObject(*master);
	assert(copyObject == *master);
	printf("assert(copyObject == *master)\n");
	SmartPointer<GameObject> differentObject(new GameObject(tag));
	assert(differentObject != *master);
	printf("assert(differentObject != *master)\n\n");

	/*
	* Should support nullptr as a value for the underlying pointer.
	*/

	master = new SmartPointer<GameObject>(NULL);
	SmartPointer<GameObject> nullCopy(*master);
	assert(master->getRefCounts().strongCount == 2); //make sure this is still a a valid smart pointer
	printf("assert(master->getRefCount() == 2)\n\n");

	/*
	* Should support default creation (i.e. unassigned smart pointers 
	*/
	SmartPointer<GameObject> defaultPointer;
	assert(defaultPointer.getRefCounts().strongCount == 0);
	assert(defaultPointer.operator->() == nullptr);
	printf("assert(defaultPointer.getRefCount() == 0)\n");
	printf("assert(defaultPointer.operator->() == nullptr)\n\n");

	defaultPointer = *master;
	assert(defaultPointer.getRefCounts().strongCount == master->getRefCounts().strongCount);
	assert(defaultPointer == *master);

	delete master;

	system("pause");
}



