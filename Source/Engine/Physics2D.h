#pragma once
#include "PhysicsComponent.h"
#include "Matrix4.h"
#include <stdio.h>
#include <winnt.h>
#include <string.h>
#include <vector>
#include "GameObject.h"
#include "SmartPointer.h"

namespace Physics2D {

	//timer stuff
	LARGE_INTEGER startOfFrame, endOfFrame, frame_time;
	LARGE_INTEGER frameFrequency;

	long long frame_rate;
	long long debug_frame_time = 6;
	const int maxFrames = 100;
	int currentFrame = 0;
	long long pastFrames[100];

	bool printFrame = true;
	bool printPhysics = false;

	float globalX = 600.0f;
	float globalY = 800.0f;

	//test physics
	static const float gravity = -0.0095f;

	enum Direction { left, right, up, down, none };

	static long long calculateFrameRate() {

		//frame rate = 1.0/avg_frame_time

		//calculate average frame time
		float avg_frame_time = -1;

		for (int i = 0; i < maxFrames; i++) {

			avg_frame_time += pastFrames[i];
		}

		avg_frame_time = avg_frame_time / maxFrames;

		return static_cast<long long>( (1 / avg_frame_time) *  1000);
	}

	static void ResetPhysics(PhysicsComponent * physics) {
		physics->transform.position.x = 0;
		physics->velocity.x = 0;
		physics->transform.position.y = 0;
		physics->velocity.y = 0;
		physics->Xforce = 0;
		physics->Yforce = 0;
	}

	static long long FrameTime() {

#ifdef _DEBUG
		return debug_frame_time;
#endif
		return frame_time.QuadPart;


	}

	static void DoEuler(const Math::Vector4 &direction, PhysicsComponent * &physics) {

		//apply physics?
		if (physics->applyGravity) {
			physics->Xforce += physics->appliedForce * direction.x;
			physics->Yforce += physics->appliedForce * direction.y;
			physics->Yforce += gravity;
		}
		else {
			physics->Xforce = physics->appliedForce * direction.x;
			physics->Yforce = physics->appliedForce * direction.y;
		}

#ifdef _DEBUG
		physics->transform.position.x = physics->transform.position.x + physics->velocity.x * debug_frame_time;
		physics->transform.position.y = physics->transform.position.y + physics->velocity.y * debug_frame_time;

#else
		if (frame_time.QuadPart < debug_frame_time) {
			frame_time.QuadPart = debug_frame_time;
		}
		physics->transform.position.x = physics->transform.position.x + physics->velocity.x * frame_time.QuadPart;
		physics->transform.position.y = physics->transform.position.y + physics->velocity.y * frame_time.QuadPart;

#endif // _DEBUG
	
#ifdef _DEBUG
		physics->velocity.x = physics->velocity.x + ((physics->Xforce) / physics->mass) * debug_frame_time;
		physics->velocity.y = physics->velocity.y + ((physics->Yforce) / physics->mass) * debug_frame_time;
#else
		//get new velocity 
		if (frame_time.QuadPart < debug_frame_time) {
			frame_time.QuadPart = debug_frame_time;
		}
		physics->velocity.x = physics->velocity.x + ((physics->Xforce) / physics->mass) * frame_time.QuadPart;
		physics->velocity.y = physics->velocity.y + ((physics->Yforce) / physics->mass) * frame_time.QuadPart;
#endif // _DEBUG


		if (physics->velocity.x > 0) {
			physics->velocity.x *= physics->drag;
		}
		else if (physics->velocity.x < 0) {
			physics->velocity.x *= physics->drag;
		}
		if (physics->velocity.y > 0) {
			physics->velocity.y *= physics->drag;
		}
		else if (physics->velocity.y < 0) {
			physics->velocity.y *= physics->drag;
		}

	}

	static void DoEulerDirection(const Direction &direction, PhysicsComponent * &physics) {

		Math::Vector4 input;
		float x;
		float y;

		//apply input force
		if (direction == right) {

			x = 1.0;
		}
		else if (direction == left) {

			x = -1.0;

		}
		else if (direction != right && direction != left) {
			x = 0.0;
		}
		if (direction == up) {

			y = 1.0;

		}
		else if (direction == down) {

			y = -1.0;
		}
		else if (direction != up && direction != down) {
			y = 0.0;
		}

		if (direction == none) {

			x = 0.0;
			y = 0.0;
		}

		input = Math::Vector4(x, y, 0, 1);

		DoEuler(input, physics);

	}

	static void DoStartFrame() {

		//time stamp the beginning of the frame
		QueryPerformanceFrequency(&frameFrequency);
		QueryPerformanceCounter(&startOfFrame);

	}

	static void DoEndFrame() {

		QueryPerformanceCounter(&endOfFrame);
		frame_time.QuadPart = endOfFrame.QuadPart - startOfFrame.QuadPart;
		frame_time.QuadPart *= 1000;
		frame_time.QuadPart /= frameFrequency.QuadPart;

		pastFrames[currentFrame] = frame_time.QuadPart;
		currentFrame++;
		if (currentFrame == maxFrames) {
			currentFrame = 0;
			frame_rate = calculateFrameRate();
		}

	}

	class PhysicsManager {

	private:
		//we only support this many collidables at a time
		std::string addPhysicsString = "AddPhysics";
		std::string removePhysicsString = "RemovePhysics";

	public:

		std::vector<SmartPointer<GameObject> *> physicsObjects;
		size_t addHash;
		size_t remHash;

		PhysicsManager()  {

			physicsObjects = std::vector<SmartPointer<GameObject> *>();
			addHash = std::hash<std::string>{}(addPhysicsString);
			remHash = std::hash<std::string>{}(removePhysicsString);

		}

		~PhysicsManager() {
			
		}

		bool AddPhysicsObject(SmartPointer<GameObject> * &A) {


			physicsObjects.push_back(A);
			return true;


		}

		bool RemovePhysicsObject(SmartPointer<GameObject> * &A) {

			if (physicsObjects.size() <= 0) {

				return false;
			}
			else {
				for (size_t i = 0; i < physicsObjects.size(); i++) {

					if (GameObject::Equals(*(*A).operator->(), *(*physicsObjects[i]).operator->())) {
						physicsObjects[i] = NULL;
						return true;
					}
				}

				return false;
			}
		}

		bool Clear() {

			physicsObjects.clear();
		}

		void DoPhysics() {

			for (size_t i = 0; i < physicsObjects.size(); i++) {

				Physics2D::DoEulerDirection(Physics2D::Direction::none, (*physicsObjects[i])->physics);
			}
		}
	};
	static PhysicsManager _PhysicsManager = PhysicsManager();

	static void DisposePhysicsManager() {

		_PhysicsManager.~PhysicsManager();
	}

}