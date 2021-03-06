// MemoryLeakTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//just a way to test headers to see if they're causing memory leaks
#include "Source/Engine/Physics2D.h"
//#include "Source/Engine/GameObject.h"
//#include "Source/Engine/Vector3.h"
//#include "Source/Engine/LuaTools.h"
//#include "Source/Engine/Collision.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif // _DEBUG

int main()
{

	//MessagingSystem::DisposeMessagingSystem();
	//Math::Collision::DisposeCollisionManager();
	Physics2D::DisposePhysicsManager();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

    return 0;
}

