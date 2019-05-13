#pragma once
#include "Point2D.h";
#include "GameObject.h";

//Base Class
class ActorController
{
public:
	ActorController() {}
	virtual ~ActorController() {} 			  
	virtual void MoveInDirection() = 0;
};

//Derived Classes
class PlayerController : public ActorController 
{
public:
	void MoveInDirection(Point2D &origin, const Point2D &direction, int x_magnitude, int y_magnitude) {

		if (direction.x == -1) {

			origin.x -= x_magnitude;

		}
		else if (direction.x == 1) {

			origin.x += x_magnitude;

		}
		
		if (direction.y == -1) {

			origin.y -= y_magnitude;

		}
		else if (direction.y == 1) {

			origin.y -= y_magnitude;

		}
	}

};



