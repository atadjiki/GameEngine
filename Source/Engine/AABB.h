#pragma once
#include "Matrix4.h"

static bool debug = false;

namespace Math {

	class AABB
	{
	public:

		Vector4 center;
		Vector4 extents;

		AABB() {

			center = Vector4(0, 0, 0, 0);
			extents = Vector4(0, 0, 0, 0);

		}

		AABB(const Vector4 &_center, const Vector4 &_extents) {

			center = Vector4(_center);
			extents = Vector4(_extents);

		}

		AABB(const float &cx, const float &cy, const float &ex, const float &ey) {

			center = Vector4(cx, cy, 0, 0);
			extents = Vector4(ex, ey, 0, 0);
		}

		~AABB() {
		
			center.~Vector4();
			extents.~Vector4();
		}

		void printBounds() {

			printf("Center: ");
			center.printVector();
			printf("Bounds: ");
			extents.printVector();

		}
	};

}