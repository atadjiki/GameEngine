#pragma once
#include "External/GLib/GLib.h"
#include <math.h>
class Vector2
{
public:

	float x;
	float y;

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}
	~Vector2() {

	}

	GLib::Point2D toPoint2D() {

		GLib::Point2D result;
		result.x = x;

		return result;
	}

	float DistanceTo(const Vector2 &target) {

		float x_dist = pow((target.x - x), 2);
		float y_dist = pow((target.y - y), 2);

		return sqrt(x_dist + y_dist);
	}

	Vector2 operator + (const Vector2 & B) {

		return Vector2(this->x + B.x, this->y + B.y);
	}
};

