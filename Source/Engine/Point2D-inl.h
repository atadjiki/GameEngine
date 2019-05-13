inline Point2D operator+(const Point2D &i_lhs, const Point2D & i_rhs) {

	Point2D temp;
	temp.x(i_lhs.x() + i_rhs.x());
	temp.y(i_lhs.y() + i_rhs.y());

	return temp;
}

inline Point2D operator-(const Point2D &i_lhs, const Point2D & i_rhs) {

	Point2D temp;
	temp.x(i_lhs.x() - i_rhs.x());
	temp.y(i_lhs.y() - i_rhs.y());

	return temp;
}

inline Point2D operator*(const Point2D &i_lhs, const Point2D & i_rhs) {

	Point2D temp;
	temp.x(i_lhs.x() * i_rhs.x());
	temp.y(i_lhs.y() * i_rhs.y());

	return temp;
}

inline Point2D operator/(const Point2D &i_lhs, const Point2D & i_rhs) {

	Point2D temp;
	temp.x(i_lhs.x() / i_rhs.x());
	temp.y(i_lhs.y() / i_rhs.y());

	return temp;
}

inline bool operator==(const Point2D &i_lhs, const Point2D & i_rhs) {

	if (i_lhs.x() == i_rhs.x()) {
		if (i_lhs.y() == i_rhs.y()) {
			return true;
		}
	}

	return false;
	

}

inline Point2D operator+=(const Point2D &i_lhs, const Point2D & i_rhs) {

	return i_lhs + i_rhs;
}

inline Point2D operator-=(const Point2D &i_lhs, const Point2D & i_rhs) {

	return i_lhs - i_rhs;
}
