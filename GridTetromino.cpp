#include "GridTetromino.h"
#include "Tetromino.h"
#include "Point.h"


//Constructors
GridTetromino::GridTetromino(){ 
	gridLoc={0,0};
}
GridTetromino::GridTetromino(Point gridLoc) {
	this->gridLoc = gridLoc;
}


// return the tetromino's grid/gameboard loc (x,y)
Point GridTetromino::getGridLoc() const {
	return gridLoc;
};
// sets the tetromino's grid/gameboard loc using x,y
void GridTetromino::setGridLoc(int x, int y) {
	gridLoc.setXY(x, y);
};
// sets the tetromino's grid/gameboard loc using a Point
void GridTetromino::setGridLoc(const Point& pt) {
	gridLoc.setXY(pt.getX(),pt.getY());
};

// transpose the gridLoc of this shape
//	(1,0) represents a move to the right (x+1)
//	(-1,0) represents a move to the left (x-1)
//	(0,1) represents a move down (y+1)
void GridTetromino::move(int xOffset, int yOffset) {
	gridLoc.setX(xOffset += gridLoc.getX());
	gridLoc.setY(yOffset += gridLoc.getY());
};

// build and return a vector of Points to represent our inherited
// blockLocs vector mapped to the gridLoc of this object instance.
// You will need to provide this class access to blockLocs (from the Tetromino class).
// eg: if we have a Point [x,y] in our vector,
// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
//Inherits proper tetromino position from gridLoc and maps tetromino (blockLocs) around that point
std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const { 
	std::vector<Point> gridPoints;
	int x = gridLoc.getX();
	int y = gridLoc.getY();

	for (Point workingPoint : Tetromino::blockLocs) { //add a reference (&) here?
		int additionalX = workingPoint.getX();
		int additionalY = workingPoint.getY();
		Point toAdd = Point((additionalX + x), (additionalY + y));
		gridPoints.push_back(toAdd); 
	}
	return gridPoints;
};




