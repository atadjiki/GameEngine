// ConstCorrectUnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Source/Engine/Point2D.h"

int main()
{
    std::cout << "Testing Point2D Class for Const Correctness\n";

	printf("Creating two points: \n");
	Point2D pointA = Point2D(5, 7);
	Point2D pointB = Point2D(1, 1);

	std::cout << "Point A: (" << pointA.x() << "," << "" << pointA.y() << ")" << std::endl;;
	std::cout << "Point B: (" << pointB.x() << "," << "" << pointB.y() << ")" << std::endl;;

	printf("Asserting A+=B\n");
	pointA += pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Asserting A-=B\n");
	pointA -= pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Asserting A/B \n");
	pointA / pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Asserting A*B \n");
	pointA * pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Asserting A+B\n");
	pointA + pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Asserting A-B\n");
	pointA - pointB;
	assert(pointA == Point2D(5, 7)); //make sure points are still the same
	assert(pointB == Point2D(1, 1));

	printf("Class passed all assertions");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
