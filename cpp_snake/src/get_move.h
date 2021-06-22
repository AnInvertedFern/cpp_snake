/*
 * get_move.h
 *
 *  Created on: Jun 21, 2021
 */
#ifndef GET_MOVE_H
#define GET_MOVE_H

//Cardinal directions for readability
//move_type::NONE is for invalid inputs; moves the snake in the previous direction
enum class move_type {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

//getMove queries the player for a cardinal direction
move_type getMove();

#endif
