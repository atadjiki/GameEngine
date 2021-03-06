#include "pch.h"
#include <assert.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <string>

#include "Source/Engine/Physics2D.h"
#include "Source/Engine/Collision.h"


const std::string testMessage = "Test Message";

float random(float a, float b)
{
	return a + (rand() / (RAND_MAX / (b - a)));
}
int main()
{

	for (int i = 0; i < 100; i++) {

		Math::Transform transform = Math::Transform();
		
		transform.position = Math::Vector4(random(-300, 300), random(-300, 300), 0.0f);
		transform.rotation = Math::Vector4(random(-180, 180), random(-180, 180), 0.0f);
		transform.bounds = Math::AABB(0, 0, random(2, 20), random(2, 20));

		SmartPointer<GameObject> * tmp = new SmartPointer<GameObject>(new GameObject(std::to_string(i), transform, 10));
		(*tmp)->physics->mass = random(1, 250);
		(*tmp)->physics->appliedForce = random(1, 10);

		Math::Collision::_CollisionManager.AddCollidable(tmp);

	}

	printf("Creating %d game objects\n", Math::Collision::_CollisionManager.collidables.size());
	int max_frames = 1000;
	int current_frames = 0;

	float total_time = 0;

	int totalCollisions = 0;
	int totalFrames = 0;



	while (true) {

		//start frame
		Physics2D::DoStartFrame();
		float t_total = 0;
		totalFrames++;

		//run physics
		for (size_t i = 0; i < Math::Collision::_CollisionManager.collidables.size(); i++) {

			Physics2D::DoEuler(Math::Vector4(random(-10, 10), random(-10, 10), 0, 1), Math::Collision::_CollisionManager.collidables[i]->operator->()->physics);
		}

		//get all collisions for this frame
		std::vector<Math::Collision::CollisionInfo> collisions = Math::Collision::_CollisionManager.ProcessCollisions(Physics2D::FrameTime());
		int collisionCount = collisions.size();
		totalCollisions += collisionCount;

		for (int i = 0; i < collisionCount; i++) {

			//if we ran out of time, cancel
			if (t_total >= Physics2D::FrameTime()) {
				break;
			}

			if (collisions[i].hit) {

				//collisions[i].PrintInfo();

				//apply new velocities
				SmartPointer<GameObject> * A = collisions[i].collidedObjects[0];
				SmartPointer<GameObject> * B = collisions[i].collidedObjects[1];

				(*A)->physics->velocity = collisions[i].postVelocities[0].toVector3();
				(*B)->physics->velocity = collisions[i].postVelocities[1].toVector3();

				//update time
				t_total += collisions[i].collisionTime;
			}

			
		}

		if (current_frames >= max_frames) {
			break;
		}
		else {

			printf("Frame %d - Collisions - %d\n", current_frames, collisionCount);
			current_frames++;
		}
		Physics2D::DoEndFrame();
		total_time += Physics2D::FrameTime();


	}

	printf("\nTime: %f secs\n", (total_time/1000));
	printf("\n\n%d Total Collisions", totalCollisions);
	printf("\nRan for %d frames\n", totalFrames - 1);
	printf("\nAverage: %d collisions per frame\n", totalCollisions / totalFrames);


	system("pause");
}