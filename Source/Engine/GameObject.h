#pragma once
#include "Point2D.h"
#include "External/GLib/GLib.h"
#include "PhysicsComponent.h"
#include "SmartPointer.h"
#include <string>
#include <new>

class GameObject
{

private: 
	PhysicsComponent * physAlloc() {
		return (PhysicsComponent *)_aligned_malloc(sizeof(PhysicsComponent), 16);
	}

public:
	
	std::string tag;
	PhysicsComponent * physics;

	GameObject(std::string i_tag, GLib::Point2D i_pos, int i_health) {

		tag = i_tag;
		physics = physAlloc();
		physics->transform.position.x = i_pos.x;
		physics->transform.position.y = i_pos.y;
	}

	GameObject(std::string i_tag, Math::Transform i_transform, int i_health) {

		tag = i_tag;
		physics = physAlloc();
		physics->transform = i_transform;
	}

	GameObject(std::string i_tag, Vector3 i_pos, int i_health) {

		tag = i_tag;
		physics = physAlloc();
		physics->transform.position = Math::Vector4(i_pos);
	}

	GameObject(std::string i_tag, Math::Vector4 i_pos, int i_health) {

		tag = i_tag;
		physics = physAlloc();
		physics->transform.position = i_pos;
	}

	GameObject(std::string i_tag) {

		tag = i_tag;
		physics = physAlloc();
	}

	GameObject() {

		tag = "GameObject";
		physics = physAlloc();
	}

	~GameObject() {

		tag.~basic_string();
		_aligned_free(physics);
		
	}

	static bool Equals (const GameObject &A, const GameObject &B) {

		if (A.tag == B.tag && A.physics == B.physics) {
			return true;
		}
		return false;
	}
};

