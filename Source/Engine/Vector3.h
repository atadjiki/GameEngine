#pragma once
#include "External/GLib/GLib.h"
class Vector3
{
	public:

	float x;
	float z;
	float y;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	~Vector3() {

	}

	GLib::Point2D toPoint2D() {

		GLib::Point2D result;
		result.x = x;
		result.y = y;

		return result;
	}


};

