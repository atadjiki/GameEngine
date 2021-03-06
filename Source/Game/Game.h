#pragma once
#ifndef GAME
#define GAME

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif // _DEBUG

#include "Source/Engine/GLibRender.h"
#include "Source/Engine/Physics2D.h"
#include "Source/Engine/GameObject.h"
#include "Source/Engine/Vector3.h"
#include "Source/Engine/LuaTools.h"
#include "Source/Engine/Collision.h"

#include "External/GLib/GLib.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <vector>
#include <iostream>

//dimensions
const float globalX = 600.0f;
const float globalY = 600.0f;

//input variables
static int playerRight = 0;
bool bQuit;
bool bSpace;

//sprite pointers
GLib::Sprites::Sprite * pMan;
GLib::Sprites::Sprite * pCat;
GLib::Sprites::Sprite * pStart;
GLib::Sprites::Sprite * pEnd;
GLib::Sprites::Sprite * pBackground;

//game objects
SmartPointer<GameObject> * player;
SmartPointer<GameObject> ** cats;
SmartPointer<GameObject> * killbox; //way to auto-kill the falling cats
int catCount = 0;
const int maxCats = 3;

//functions
void LoadSprites();
void Startup();
void Cleanup();
void PlayerSetup();
void KillBoxSetup();

//Structure of game
void RunStartMenu();
void RunGameLoop();
void RunEndMenu();

//Utility
Physics2D::Direction GetInput();
void DrawAll();

//Logic
bool ValidatePlayerPosition(Physics2D::Direction direction);
void SpawnCat();
void ResetCat(SmartPointer<GameObject> *);

#include "resource.h"
#endif // !GAME