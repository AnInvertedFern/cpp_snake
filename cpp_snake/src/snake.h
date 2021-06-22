/*
 * snake.h
 *
 *  Created on: Jun 21, 2021
 */
#ifndef SNAKE_H_
#define SNAKE_H_
#include "get_move.h"
#include <vector>

//Decelerations of the getters for tile types
char getEdgeChar();
char getBlankChar();
char getFoodChar();
char getEnemChar();

char getSnekChar();  //Body of snek
char getHeadChar();
char getDeadChar();  //For non-enemy kills
char getKillChar();  //For enemy kills

class Snake {

	//ALL LOCATIONS ARE IN FLATTENED COORDINATES

private:
	//Previous head direction
	move_type m_head_direction {move_type::UP};
	//Holds the locations of all segments of the snake
	std::vector<int> m_snake_loc {0};
	//Game board, but is flattened
	//Holds the tile information for printing
	//State information is held in the other functions
	char* m_board;
	//Length of one dimension of the board
	//Board is square, because well maybe I'll fix it later
	int m_board_size;
	//There is only one food at a time, might change that later
	int m_food_loc {0};
	//Each item in the m_enem_loc vector is one enemies' position
	std::vector<int> m_enem_loc {0};
public:
	Snake(int boardSize);
	~Snake();
	//Returns false if the snake has died
	//Returns true otherwise
	//Does a game move
	bool MoveSnake(move_type head_direction);
	//Prints the board to console
	void PrintBoard();
};



#endif /* SNAKE_H_ */
