/*
 * get_move.cpp
 *
 *  Created on: Jun 21, 2021
 */
#include <iostream>
#include <string>
#include "get_move.h"

//getMove queries the player for a cardinal direction
move_type getMove() {
	//Only "up", "down", "left", "right" are valid
	//All else are defaulted to the previous input in the Snake class
	std::cout << "Choose a Direction (with lowercase words):" << std::endl;
	std::string str_direction;
	std::getline(std::cin, str_direction);

	//I think getline() is picking up the '\n'
	//The standard methods did not seem to work, so i just cut it off
	//Seems fine, hope it does not mess with something
	//Or make the wrong assumptions
	str_direction = str_direction.substr(0, str_direction.length()-1);

	move_type direction;
	//switch(str_direction) {   // No switching for strings

	if (str_direction.compare("up") == 0)
		direction = move_type::UP;
	else if (str_direction.compare("down") == 0)
		direction = move_type::DOWN;
	else if (str_direction.compare("left") == 0)
		direction = move_type::LEFT;
	else if (str_direction.compare("right") == 0)
		direction = move_type::RIGHT;
	else
		direction = move_type::NONE;

	return direction;


}


