/*
 * cpp_snake.cpp
 *
 *  Created on: Jun 21, 2021
 *
 *  This program plays a game of snek, except there are enemies or something
 */
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "get_move.h"
#include "snake.h"

int main() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	std::cout << "THIS IS A GAME OF SNAKE!!!" << std::endl;
	std::cout << "PLEASE INPUT BOARD SIZE:  " << std::endl;

	//I could take all this initial input dialog logic and put in a separate function
	//But I don't like making functions I have no interest in reusing
	int boardSize;
	std::cin >> boardSize;
	if(std::cin.fail()) {
		std::cout << "I'm setting it to 10" << std::endl;
		boardSize = 10;
		std::cin.clear();
	}
	//Don't care about other things on the end of numbers
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	if (boardSize >= 50) {
		std::cout << "That board size is too big, I'm making it 10"<< std::endl;
		boardSize = 10;
	}
	if (boardSize < 5) {
		std::cout << "That board size is too small, I'm making it 10"<< std::endl;
		boardSize = 10;
	}

	//Board Tile Chars defined in snake.cpp
	std::cout << "The board size is: "<< boardSize << std::endl;
	std::cout << getBlankChar()<< ": are empty space" << std::endl;
	std::cout << getFoodChar()<< ": are food" << std::endl;
	std::cout << getEnemChar()<< ": are enemies" << std::endl;
	std::cout << getSnekChar()<< ": are the snake's body" << std::endl;
	std::cout << getHeadChar()<< ": are the snake's head" << std::endl;

	//Class Snake holds all game functionality
	Snake game = Snake(boardSize);
	game.PrintBoard();


	move_type direction {};
	bool alive {true};
	while (alive){
		//getMove queries the player for a cardinal direction
		direction = getMove();
		alive = game.MoveSnake(direction);
		game.PrintBoard();
	}

	std::cout << "ERR, Snek Dead" << std::endl;

	return 0;
}
