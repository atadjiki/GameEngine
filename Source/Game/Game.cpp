//#define NDEBUG

#include "stdafx.h"
#include "Game.h"

void KeyCallback(unsigned int i_VKeyID, bool bWentDown)
{

	if (i_VKeyID == 0x41 && bWentDown) { //A Key

		playerRight = -1;
	}
	else if (i_VKeyID == 0x44 && bWentDown) { //D Key

		playerRight = 1;
	}
	else {
		playerRight = 0;
	}

	if (i_VKeyID == 0x1B && bWentDown) { //ESC key

		bQuit = true;
	}

	if (i_VKeyID == 0x20 && bWentDown) { //SPACE key

		bSpace = true;
	}
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "Raining Cats", -1, 600, 600);

	if (bSuccess)
	{
		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(KeyCallback);

		Startup();

		RunStartMenu();

		RunGameLoop();

		RunEndMenu();

		Cleanup();

		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();
	}

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

}

/*
	Start menu, Game, and End menu
*/
void RunStartMenu() {

	bSpace = false;
	bQuit = false;

	do {
		GLib::Service(bQuit);
		BeginRender();
		DoRender(pStart, Vector3(0,-300,0).toPoint2D(), 0.0f);
		EndRender();

	} while (bSpace == false);

	return;

}

void RunGameLoop() {

	do
	{
		GLib::Service(bQuit);
		Physics2D::DoStartFrame();

		//run collision
		std::vector<Math::Collision::CollisionInfo> collisions = Math::Collision::_CollisionManager.ProcessCollisions(Physics2D::FrameTime());
		for (size_t i = 0; i < collisions.size(); i++) {

			if ((*collisions[i].collidedObjects[0])->tag == (*player)->tag || (*collisions[i].collidedObjects[1])->tag == (*player)->tag && collisions[i].hit) {

				if ((*collisions[i].collidedObjects[0])->tag == "Cat" || (*collisions[i].collidedObjects[1])->tag == "Cat") {
					Math::Vector4 col0 = (*collisions[i].collidedObjects[0])->physics->transform.position;
					Math::Vector4 col1 = (*collisions[i].collidedObjects[1])->physics->transform.position;
					return;
				}
			}else if ((*collisions[i].collidedObjects[0])->tag == (*killbox)->tag || (*collisions[i].collidedObjects[1])->tag == (*killbox)->tag) {
				Math::Vector4 col0 = (*collisions[i].collidedObjects[0])->physics->transform.position;
				Math::Vector4 col1 = (*collisions[i].collidedObjects[1])->physics->transform.position;

				if ((*collisions[i].collidedObjects[0])->tag == "Cat") {
					ResetCat((collisions[i].collidedObjects[0]));
				}
				else {
					ResetCat((collisions[i].collidedObjects[1]));
				}

				SpawnCat();
				
			}

		}

		//do registered physics
		Physics2D::_PhysicsManager.DoPhysics();

		//do Player Physics
		Physics2D::DoEulerDirection(GetInput(), (*player)->physics);

		//draw 
		DrawAll();

		//Get delta time
		Physics2D::DoEndFrame();

	} while (bQuit == false);

	return;
}

void RunEndMenu() {

	bQuit = false;
	bSpace = false;

	do {
		GLib::Service(bQuit);
		BeginRender();
		DoRender(pEnd, Vector3(0, -300, 0).toPoint2D(), 0.0f);
		EndRender();
		 
 	} while (bSpace == false);

	
}

/*
	Game Logic
*/
bool ValidatePlayerPosition(Physics2D::Direction direction) {

	if (direction == Physics2D::Direction::right) {
		if ((*player)->physics->transform.position.x >= 250) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (direction == Physics2D::Direction::left) {
		if ((*player)->physics->transform.position.x <= -250) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

//Render Functions

void DrawAll() {

	BeginRender();

	if (pCat) {
		
		for (int i = 0; i < catCount; i++) {

			DoRender(pCat, (*cats[i])->physics->transform.position.toPoint2D(), (*cats[i])->physics->transform.rotation.Magnitude());
		}
	}

	if (pMan){

		// Tell GLib to render this sprite at our calculated location
		DoRender(pMan, (*player)->physics->transform.position.toPoint2D(), 0.0f);
	
	}

	if (pBackground) {

		DoRender(pBackground, Vector3(0, -300, 0).toPoint2D(), 0.0f);

	}

	EndRender();

}

/*
	Init/Cleanup Functions
*/

void SpawnCat() {

	if (catCount == maxCats) {
		return;
	}

	cats[catCount] = CreateSmartPointerFromFile("data\\CreateProjectile.lua");

	ResetCat(cats[catCount]);

	Math::Collision::_CollisionManager.AddCollidable(cats[catCount]);

	Physics2D::_PhysicsManager.AddPhysicsObject(cats[catCount]);

	catCount++;

}

void ResetCat(SmartPointer<GameObject> * cat) {

	/*
		int range = max - min + 1;
		int num = rand() % range + min;
	*/

	(*cat)->physics->applyGravity = true;
	(*cat)->physics->velocity = Vector3();
	(*cat)->physics->Yforce = -10.0f;
	(*cat)->physics->transform.position.x = rand() % (300 - -300 + 1) + -300;
	(*cat)->physics->transform.position.y = 310;
	(*cat)->physics->transform.rotation.x = rand() % 360 + 0;
	(*cat)->physics->transform.bounds.center = Vector3(0, 0, 0);
	(*cat)->physics->transform.bounds.extents = Vector3(50, 50, 0);
	(*cat)->physics->appliedForce = rand() % 25 + 15;
}

void Startup() {

	srand(1);
	cats = new SmartPointer<GameObject>*[maxCats];

	// Load sprites
	LoadSprites();

	//Load game objects from .lua files
	PlayerSetup();

	SpawnCat();

	KillBoxSetup();
	
	bQuit = false;
	bSpace = false;
}

void LoadSprites() {
	pMan = CreateSprite("data\\sprites\\man.DDS");
	pCat = CreateSprite("data\\sprites\\cat.DDS");
	pStart = CreateSprite("data\\sprites\\start.DDS");
	pEnd = CreateSprite("data\\sprites\\end.DDS");
	pBackground = CreateSprite("data\\sprites\\background.DDS");

	if (pMan == NULL || pCat == NULL || pStart == NULL || pEnd == NULL || pBackground == NULL) { exit(EXIT_FAILURE); }
}

void PlayerSetup() {
	player = CreateSmartPointerFromFile("data\\CreatePlayer.lua");
	(*player)->physics->applyGravity = false;
	(*player)->physics->transform.bounds.center = Vector3(0, 0, 0);
	(*player)->physics->transform.bounds.extents = Vector3(50, 100, 0);
	Math::Collision::_CollisionManager.AddCollidable(player);
}

void KillBoxSetup() {
	killbox = CreateSmartPointerFromFile("data\\CreateKillBox.lua");
	(*killbox)->physics->applyGravity = false;
	(*killbox)->physics->transform.bounds.center = Vector3(0, 0, 0);
	(*killbox)->physics->transform.bounds.extents = Vector3(600, 10, 0);
	Math::Collision::_CollisionManager.AddCollidable(killbox);
}

void Cleanup() {

	if (pMan)
		GLib::Sprites::Release(pMan);
	if (pCat)
		GLib::Sprites::Release(pCat);
	if (pStart)
		GLib::Sprites::Release(pStart);
	if (pEnd)
		GLib::Sprites::Release(pEnd);
	if (pBackground)
		GLib::Sprites::Release(pBackground);

	player->~SmartPointer();
	delete player;

	killbox->~SmartPointer();
	delete killbox;

	for (int i = 0; i < catCount; i++) {
		cats[i]->~SmartPointer();
		delete cats[i];
	}
	delete[] cats;

	MessagingSystem::DisposeMessagingSystem();
	Math::Collision::DisposeCollisionManager();
	Physics2D::DisposePhysicsManager();
}

/*
	Input Functions
*/

Physics2D::Direction GetInput() {

	if (playerRight == 1) {

		if (ValidatePlayerPosition(Physics2D::Direction::right)) {
			return Physics2D::Direction::right;
		}
		else {
			return Physics2D::Direction::none;
		}

	}
	else if (playerRight == -1) {

		if (ValidatePlayerPosition(Physics2D::Direction::left)) {
			return Physics2D::Direction::left;
		}
		else {
			return Physics2D::Direction::none;
		}
	}
	else {
		return Physics2D::Direction::none;
	}


}