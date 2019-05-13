#pragma once
#include <new>
#include <iostream>
#include <cstdlib>

#include "Transform.h"
#include "AABB.h"
#include "Vector3.h"
#include "External/GLib/GLib.h"

class PhysicsComponent {

public:

	//Vector3 position;
	Math::Transform transform;
	Vector3 velocity;
	float mass;
	float drag;
	float Xforce;
	float Yforce;
	float appliedForce;
	bool applyGravity;

	PhysicsComponent() {
		transform = Math::Transform();
		velocity = Vector3();
		mass = 1000.0f;
		drag = 0.0050f;
		Xforce = 0.0f;
		Yforce = 0.0f;
		appliedForce = 1.0f;
		applyGravity = true;
	}
	~PhysicsComponent() {

		transform.~Transform();
		velocity.~Vector3();
	}

};

