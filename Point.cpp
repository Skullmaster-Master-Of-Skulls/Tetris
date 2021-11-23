#include "Point.h"
#include <sstream>

//Constructors
Point::Point() {
	x = 0;
	y = 0;
}
Point::Point(int x) {
	this->x = x;
	y = 0;
}
Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

//getters
int Point::getX() const {
	return x;
}
int Point::getY() const {
	return y;
}
                                                                                                                                                         
//setters
void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
};
void Point::setX(int x) {
	this->x = x;
}
void Point::setY(int y) {
	this->y = y;
}

// swap x and y
void Point::swapXY() {
	int temp = x;
	x = y;
	y = temp;
};

// multiply x by some factor
void Point::multiplyX(int factor) {
	x = x * factor;
};

// multiply y by some factor
void Point::multiplyY(int factor) {
	y = y * factor;
};

// return a string in the form "[x,y]" to represent the state of the Point instance
// (for debugging) You could use stringstream if you want to try a forming a string in
// the same way you use cout http://stev.org/post/cstringstreamexample for an example
std::string Point::toString() const {
	std::stringstream ss;
	ss << "[" << x << "," << y << "]";
	std::string str = ss.str();
	return str;
};

