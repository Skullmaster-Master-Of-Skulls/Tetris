#pragma once
#include <iostream>

class Point {
	friend class TestSuite;
	friend class Gameboard;
	friend class Tetromino;
private:
	int x, y;
	
public:
	Point();
	Point(int x);
	Point(int x, int y);
	//Get x value for point
	int getX() const;
	//get Y value for point
	int getY() const;
	//set values for point
	void setXY(int x, int y);
	//set value for x
	void setX(int x);
	//set value for y
	void setY(int y);
	// swap x and y
	void swapXY();
	// multiply x by some factor
	void multiplyX(int factor);
	// multiply y by some factor
	void multiplyY(int factor);
	// return a string in the form "[x,y]" to represent the state of the Point instance
	// (for debugging) You could use stringstream if you want to try a forming a string in
	// the same way you use cout http://stev.org/post/cstringstreamexample for an example
	std::string toString() const;
};







