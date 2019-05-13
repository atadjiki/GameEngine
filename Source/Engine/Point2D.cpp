//#include "Point2D.h"
//
//Point2D operator+(const Point2D &i_lhs, const Point2D & i_rhs) {
//
//	Point2D temp;
//	temp.x(i_lhs.x() + i_rhs.x());
//	temp.y(i_lhs.y() + i_rhs.y());
//
//	return temp;
//}
//
//Point2D operator-(const Point2D &i_lhs, const Point2D & i_rhs) {
//
//	Point2D temp;
//	temp.x(i_lhs.x() - i_rhs.x());
//	temp.y(i_lhs.y() - i_rhs.y());
//
//	return temp;
//}
//
//Point2D operator*(const Point2D &i_lhs, const Point2D & i_rhs) {
//
//	Point2D temp;
//	temp.x(i_lhs.x() * i_rhs.x());
//	temp.y(i_lhs.y() * i_rhs.y());
//
//	return temp;
//}
//
//Point2D operator/(const Point2D &i_lhs, const Point2D & i_rhs) {
//
//	Point2D temp;
//	temp.x(i_lhs.x() / i_rhs.x());
//	temp.y(i_lhs.y() / i_rhs.y());
//
//	return temp;
//}
//
//bool operator==(const Point2D &i_lhs, const Point2D & i_rhs) {
//
//	if (i_lhs.x() == i_rhs.x()) {
//		if (i_lhs.y() == i_rhs.y()) {
//			return true;
//		}
//	}
//	else {
//		return false;
//	}
//
//
//}