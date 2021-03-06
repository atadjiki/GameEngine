// AABBTest.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <assert.h>

#include "Source/Engine/Physics2D.h"
#include "Source/Engine/Collision.h"

using namespace Math;

int main()
{

	float extent = 2.5;

	Transform t_A, t_B;

	t_A.position = Vector4(-10, 0, 0, 0);
	t_A.rotation = Vector4(0, 45, 0, 0);
	t_A.bounds = AABB(0, 0, extent, extent);

	t_B.position = Vector4(0, 0, 0, 0);
	t_B.rotation = Vector4(0, 90, 0, 0);
	t_B.bounds = AABB(0, 0, extent, extent);

	SmartPointer<GameObject> * A = new SmartPointer<GameObject>(new GameObject("A", t_A, 10));
	SmartPointer<GameObject> * B = new SmartPointer<GameObject>(new GameObject("B", t_B, 10));

	(*A)->physics->mass = 50;
	(*B)->physics->mass = 50;

	float max_frames = 600;
	float current_frames = 0;

	float total_time = 0;
	
	while (true) {

		__int64 collision_time = -1;

		Physics2D::DoStartFrame();

		Physics2D::DoEulerDirection(Physics2D::right, (*A)->physics);
		Physics2D::DoEulerDirection(Physics2D::left, (*B)->physics);

		if (current_frames >= max_frames) {
			printf("\nA is at : "); (*A)->physics->transform.position.printVector();
			printf("\nB is at : "); (*B)->physics->transform.position.printVector();
			printf("\nTime: %f ms\n\n", total_time);
			current_frames = 0;
		}
		else {
			current_frames++;
		}

		collision_time = Math::Collision::CheckCollisionsInFrame(A, B, Physics2D::FrameTime()).collisionTime;
		total_time += Physics2D::FrameTime();

		if (collision_time >= 0) {
			if (Math::Collision::CheckForCollision((*A)->physics->transform, (*B)->physics->transform)) {
				break;
			}
		}
		else if ((*A)->physics->transform.position.DistanceTo((*B)->physics->transform.position) < extent) {

			printf("\nEnd\n");
			system("pause");
			return 0;
		}

		Physics2D::DoEndFrame();
	}

	printf("\n\n\nCollision detected!\n");
	printf("\nA is at : "); (*A)->physics->transform.position.printVector();
	printf("\nB is at : "); (*B)->physics->transform.position.printVector();
	printf("\nA,B are %f units apart\n", (*A)->physics->transform.position.DistanceTo((*B)->physics->transform.position));
	printf("\nCollision at time: %f ms\n\n", total_time);

	system("pause");
	return 0;
}