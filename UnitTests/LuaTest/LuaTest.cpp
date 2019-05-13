// LuaTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Source/Engine/LuaTools.h"
#include "Source/Engine/GameObject.h"
#include "Source/Engine/SmartPointer.h"

int main()
{
	SmartPointer<GameObject> * gameObject = CreateSmartPointerFromFile("data\\CreateGameObject.lua");

	gameObject->~SmartPointer();

	system("pause");
}