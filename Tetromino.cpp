#include "Tetromino.h"
#include "Point.h"

Tetromino::Tetromino() {
	setShape(TetShape::I);
}
Tetromino::Tetromino(TetShape shape) {
	setShape(shape);
}

//Gets colour of TetColour
TetColour Tetromino::getColour() const {
	return colour;
};
//Gets shape of Tetromino
TetShape Tetromino::getShape() const {
	return shape;
};

// set the shape
// - clear any blockLocs set previously
// - set the blockLocs for the shape
// - set the colour for the shape
void Tetromino::setShape(TetShape shape) {
	blockLocs.clear();

	if (shape == TetShape::S) {
		blockLocs = { Point(0,0),Point(-1,0),Point(0,1),
		Point(1,1) };
		colour = TetColour::RED;
	}
	if (shape == TetShape::Z) {
		blockLocs = { Point(0,0),Point(1,0),Point(0,1),
		Point(-1,1) };
		colour = TetColour::ORANGE;
	}
	if (shape == TetShape::L) {
		blockLocs = { Point(0,0),Point(0,1),Point(0,-1),
		Point(-1,-1) };
		colour = TetColour::YELLOW;
	}
	if (shape == TetShape::J) {
		blockLocs = { Point(0,0),Point(0,1),Point(0,-1),
		Point(-1,-1) };
		colour = TetColour::GREEN;
	}
	if (shape == TetShape::O) {
		blockLocs = { Point(0,0),Point(0,1),Point(1,1),
		Point(1,0) };
		colour = TetColour::BLUE_LIGHT;
	}
	if (shape == TetShape::I) {
		blockLocs = { Point(0,0),Point(0,-1),Point(0,1),
		Point(0,2) };
		colour = TetColour::BLUE_DARK;
	}
	if (shape == TetShape::T) {
		blockLocs = { Point(-1,0),Point(0,-1),Point(0,0),
		Point(1,0) };
		colour = TetColour::PURPLE;
	}
	if (shape == TetShape::COUNT) {
		blockLocs = { Point(-1,0),Point(0,-1),Point(0,1),
		Point(1,0) };
		colour = TetColour::PURPLE;
	}

};
// rotate the shape 90 degrees around [0,0] (clockwise)
// to do this:
// - iterate through blockLocs
// - rotate each Point 90 degrees around [0,0]
// hint: rotate the point [1,2] clockwise around [0,0] and note
// how the x,y values change. There are 2 functions in the Point
// class that can be used to accomplish a rotation.
void Tetromino::rotateClockwise() {
	for (int i = 0; i < 4; i++) {
		blockLocs[i].multiplyX(-1);//Giving errors for some fucking reason?? Vector class from C++ is saying that since debug flag is >0 that it's not going to let it do this
		blockLocs[i].swapXY();
	}
};
// print a grid to display the current shape
// to do this: print out a “grid” of text to represent a co-ordinate
// system. Start at top left [-3,3] go to bottom right [3,-3]
// (use nested for loops)
// for each [x,y] point, loop through the blockLocs and if the point exists
// in the list, print an 'x' instead of a '.'. You should end up with something
// like this: (results will vary depending on shape and rotation, eg: this
// one shows a T shape rotated clockwise once)
// .......
// .......
// ...x...
// ..xx...
// ...x...
// .......
void Tetromino::printToConsole() const { //from top left to bottom right
	for (int i = -3; i <= 3; i++){ //x axis
		for (int j = 3; j >= -3; j--) { //y axis
			for (int o = 0; o <= 4; o++) {
				if (blockLocs[o].getX() == i && blockLocs[o].getY() == j) {
					std::cout << "X";
				}
				else {
					std::cout << ".";
				}
			}
		}
		std::cout << "\n";
	}
};

TetShape Tetromino::getRandomShape() {
	int shape = rand() % static_cast<int>(TetShape::COUNT);
	return(static_cast<TetShape>(shape));
}



