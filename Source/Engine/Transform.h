#pragma once
#include "Matrix4.h"
#include "AABB.h"
namespace Math {
	struct Transform
	{
		Vector4 rotation;
		Vector4 position;
		AABB bounds;

		~Transform() {

			rotation.~Vector4();
			position.~Vector4();
			bounds.~AABB();
		}
	};
}