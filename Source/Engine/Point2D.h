#pragma once
#ifndef __POINT2D
#define __POINT2D
class Point2D {

public:

	Point2D() {}

	Point2D(int i_x, int i_y) {

		m_x = i_x;
		m_y = i_y;

	}

	//get
	int x() const { return m_x; }
	int y() const { return m_y; }

	//set
	void x(int i_x) { m_x = i_x; }
	void y(int i_y) { m_y = i_y; }

	Point2D operator+(const Point2D & i_other) {

		Point2D temp;
		temp.x(x() + i_other.x());
		temp.y(y() + i_other.y());
		return temp;
	}

	Point2D operator-(const Point2D & i_other) {

		Point2D temp;
		temp.x(x() - i_other.x());
		temp.y(y() - i_other.y());
		return temp;
	}

	Point2D operator*(const Point2D & i_other) {

		Point2D temp;
		temp.x(x() * i_other.x());
		temp.y(y() * i_other.y());
		return temp;
	}

	Point2D operator/(const Point2D & i_other) {

		Point2D temp;
		temp.x(x() * i_other.x());
		temp.y(y() * i_other.y());
		return temp;
	}

	Point2D operator=(const Point2D & i_other) {

		Point2D temp;
		temp.x(i_other.x());
		temp.y(i_other.y());
		return temp;
	}

	Point2D operator++() {

		Point2D temp;
		temp.x(x() + 1);
		temp.y(y() + 1);
		return temp;
	}

	Point2D operator--() {

		Point2D temp;
		temp.x(x() - 1);
		temp.y(y() - 1);
		return temp;
	}

private:

	int m_x, m_y;
};

#include "Point2D-inl.h"
#endif