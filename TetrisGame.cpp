
// This class encapsulates the tetris game and its drawing routines, gameplay, & control logic.
// This class was designed so with the idea of potentially instantiating 2 of them
// and have them run side by side (player vs player).
// So, anything you would need for an individual tetris game has been included here.
// Anything you might use between games (like the background, or the sprite used for 
// rendering a tetromino block) was left in main.cpp
// 
// This class is responsible for:
//   - setting up the board,
//	 - drawing game elements to the screen
//   - spawning tetrominoes,
//   - handling user input,
//   - moving and placing tetrominoes 
//
//  [expected .cpp size: ~ 275 lines]

#include "TetrisGame.h"
#include "Gameboard.h"
#include "GridTetromino.h"
#include <SFML/Graphics.hpp>
     

// MEMBER FUNCTIONS

// constructor
//   initialize variables-
//   assign variables
//   reset() the game-
//   load font from file: fonts/RedOctober.ttf-
//   setup scoreText
TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, Point gameboardOffset, Point nextShapeOffset) 
	: window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset) { 
	
	window.setTitle("Tetris 2: Electric Boogaloo");
	
	//reset the game
	static int endOfGame = 0;
	reset();

	//load font from file
	if (!scoreFont.loadFromFile("./fonts/RedOctober.ttf")) {
		scoreFont.loadFromFile("./fonts/RedOctober.ttf");
	}

	//setup scoreText
	score = 0;
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(26); // in pixels. 30 fills the mini window completely but does not account for high scores
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition(420, 330);
	window.draw(scoreText); 
	//window.setKeyRepeatEnabled(false);


	
	
	
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape,gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset); 
	window.draw(scoreText);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {
	sf::Keyboard::Key keyPressed = event.key.code;
	if (keyPressed == sf::Keyboard::Up) {
		attemptRotate(currentShape);
	}
	else if (keyPressed == sf::Keyboard::Right) {
		attemptMove(currentShape,1,0);
	}
	else if (keyPressed == sf::Keyboard::Left) {
		attemptMove(currentShape, -1, 0);
	}
	else if (keyPressed == sf::Keyboard::Down) {
		if (!attemptMove(currentShape, 0, 1)) {
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
		};
	}
	else if (keyPressed == sf::Keyboard::Space){
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {
	secondsSinceLastTick += secondsSinceLastLoop;
	if (secondsPerTick <= secondsSinceLastTick) {
		tick();
		secondsSinceLastTick -= secondsPerTick;
	}
	if (shapePlacedSinceLastGameLoop) {
		if (!spawnNextShape()) {
			reset();
		}
		score = score + board.removeCompletedRows();
		updateScoreDisplay();
		pickNextShape();
		determineSecondsPerTick(); //currently has nothing
		shapePlacedSinceLastGameLoop = false;
	}

}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further), and record the fact that a
// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {
	if(!attemptMove(currentShape, 0, 1)){
		lock(currentShape); 
		shapePlacedSinceLastGameLoop = true;
	}
}



// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
void TetrisGame::reset() {
	//endOfGame++;
	//if(endOfGame>=1){
	//int randInt = rand() % 7;
	//blockSprite.setTextureRect(sf::IntRect((BLOCK_WIDTH * randInt), 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	//fill board with blocks
	//wait for 2 seconds before reset finishes
	//}
	
	
	updateScoreDisplay();
	score = 0;
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
	
}

// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {
	nextShape.setShape(Tetromino::getRandomShape());
	//nextShape.setGridLoc(nextShapeOffset);
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() { 
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	
	std::cout << "\nnextShape X location " << nextShape.getGridLoc().getX(); //incorrect
	std::cout << "\nnextShape Y location " << nextShape.getGridLoc().getY(); //incorrect
	if (isPositionLegal(currentShape)) {
		return true;
	}
	return false;

}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (shape.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) { 
	GridTetromino tempTet = shape;
	tempTet.rotateClockwise();
	if (isPositionLegal(tempTet)) {
		shape.rotateClockwise();
		return true;
	}
	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.
//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {  
	GridTetromino tempTet = shape;
	tempTet.move(x,y);
	if (isPositionLegal(tempTet)) {
		shape.move(x, y);
		return true;
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino& shape) {
	while(attemptMove(shape, 0, 1)) {}
	//lock(currentShape);
}

// copy the contents (colour) of the tetromino's mapped block locs to the grid.
//	 1) get the tetromino's mapped locs via tetromino.getBlockLocsMappedToGrid()
//   2) make Gameboard's isValidPoint() methods public
//	 2) iterate through the mapped locations, if the location is a valid point 
//      (according to the gameboard) then use Gameboard.setContent() to set the
//      board content to be the colour of the tetromino. 
void TetrisGame::lock(const GridTetromino& shape) {
	TetColour colour = shape.getColour();
	std::vector<Point> blocksToGrid = shape.getBlockLocsMappedToGrid();
	for (Point block : blocksToGrid) {
		if (board.isValidPoint(block)) {
			board.setContent(block.getX(), block.getY(), static_cast<int>(colour)); 
		}
	}
}

// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block colour using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColour colour) {	
	blockSprite.setTextureRect(sf::IntRect((BLOCK_WIDTH*static_cast<int>(colour)), 0, BLOCK_WIDTH, BLOCK_HEIGHT)); 
	blockSprite.setPosition(topLeft.getX()+(xOffset*BLOCK_WIDTH),topLeft.getY()+(yOffset*BLOCK_HEIGHT));
	window.draw(blockSprite);
}


// Draw the gameboard blocks on the window
//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
void TetrisGame::drawGameboard() {

	for (int y = 0; y < Gameboard::MAX_Y; y++) { //top to bottom
		for (int x = 0; x < Gameboard::MAX_X; x++) { //left to right
			if (board.getContent(x,y)!= Gameboard::EMPTY_BLOCK) {
				TetColour colour = static_cast<TetColour>(board.getContent(x, y));
				drawBlock(gameboardOffset,x,y,colour);
			}
		}
	}
}



//   Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//   If the Tetromino is on the gameboard: use gameboardOffset
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) {	
	std::vector<Point> spawnLoc = tetromino.getBlockLocsMappedToGrid();
	for (Point blockToDraw : spawnLoc) { 
		int x = blockToDraw.getX();
		int y = blockToDraw.getY();
		drawBlock(topLeft, x, y, tetromino.getColour());  //spawns full tetromino in top left corner. x+5 shifts this to middle
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
void TetrisGame::updateScoreDisplay() { 
	//each tetromino placed = +1 to score. Change to every line removed?
	std::string newScore = "Score: " + std::to_string(score);
	scoreText.setString(newScore);
}

// State & gameplay/logic methods ================================

// return true if shape is within borders (isShapeWithinBorders())
//	 and the shape's mapped board locs are empty.
//   Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {  
	bool success = isShapeWithinBorders(shape) && board.areLocsEmpty(shape.getBlockLocsMappedToGrid());
	return success;
}

// return true if the shape is within the left, right, and lower border of
//	 the grid, but *NOT* the top border. (return false otherwise)
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) const { 
	std::vector<Point> mapLocs = shape.getBlockLocsMappedToGrid();
	for (Point singleBlock : mapLocs) {
		if (singleBlock.getY() >= Gameboard::MAX_Y|| singleBlock.getX() < 0 || singleBlock.getX() >= Gameboard::MAX_X) {
			return false;
		}
	}
	return true;
}


// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
void TetrisGame::determineSecondsPerTick() {}
