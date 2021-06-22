/*
 * snake.cpp
 *
 *  Created on: Jun 21, 2021
 */

#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "get_move.h"
#include "snake.h"

//Definitions of the getters for tile types
char getEdgeChar() {return '*';}
char getBlankChar() {return '_';}
char getFoodChar() {return '@';}
char getEnemChar() {return 'T';}

char getSnekChar() {return '/';}  //Body of snek
char getHeadChar() {return '&';}
char getDeadChar() {return 'X';}  //For non-enemy kills
char getKillChar() {return 'K';}  //For enemy kills

//Shamelessly copied from Alex from LearnCPP
static int getRandomNumber(int min, int max){
	static constexpr double fraction { 1.0/(RAND_MAX + 1.0)};
	return min + static_cast<int>((max-min+1)*(std::rand()*fraction));
}

Snake::Snake(int boardSize) {
	//m_board is a flattened square board of length boardSize
	m_board  = new char[boardSize*boardSize]{};  //I don't entirely know for sure if this needs to be dynamic, might fiddle with later
	m_board_size = boardSize;
	for (int i {0}; i < m_board_size*m_board_size; ++i) {
		m_board[i] = getBlankChar();   //blank tiles
	}

	//GIVES ALL INTERNAL STATES OF THE GAME RANDOM LOCATIONS
	//AND NON_RANDOM TILES

	m_snake_loc[0] = getRandomNumber( 0, m_board_size*m_board_size );
	m_board[ m_snake_loc[0] ] = getHeadChar();


	m_food_loc = getRandomNumber( 0, m_board_size*m_board_size );
	if (m_food_loc==m_snake_loc[0]){ //CHECKS THAT FOOD DOES NOT SPAWN ON THE SNAKE TO START WITH
		m_food_loc = m_food_loc + 1;
	}
	m_board[ m_food_loc ] = getFoodChar();

	m_enem_loc[0] = getRandomNumber( 0, m_board_size*m_board_size );
	if (m_enem_loc[0]==m_snake_loc[0]){ //CHECKS THAT ENEMY DOES NOT SPAWN ON THE SNAKE TO START WITH
		m_enem_loc[0] = m_enem_loc[0] +1;
	}
	m_board[ m_enem_loc[0]] = getEnemChar();



}

bool Snake::MoveSnake(move_type head_direction){
	//Modifying the parameter should be okay since it is a enum and is passed by value
	if (head_direction == move_type::NONE) head_direction = m_head_direction;  //IF INPUT WAS INVALID JUST KEEP GOING IN THE SAME DIRECTION
	int cast_dir = static_cast<int>(head_direction);//JUST FOR THE SWITCH STATMENT
	int move_to_loc{};  //THE LOCATION THE SNAKE WILL BE ON THE NEXT TICK
	int loc_x = m_snake_loc[0]%m_board_size;
	int loc_y = m_snake_loc[0]/m_board_size;
	switch (cast_dir){  //CHECK FOR OUT OF BOUNDS AND CALCULATE THE NEXT LOCATION FOR SNEK
		case 0:
		{
			if (loc_y == 0) {move_to_loc = -1; break;}
			move_to_loc = loc_x+(loc_y-1)*m_board_size;
			break;
		}
		case 1:
		{
			if (loc_y == m_board_size-1) {move_to_loc = -1; break;}
			move_to_loc = loc_x+(loc_y+1)*m_board_size;
			break;
		}
		case 2:
		{
			if (loc_x == 0) {move_to_loc = -1; break;}
			move_to_loc = (loc_x-1)+loc_y*m_board_size;
			break;
		}
		case 3:
		{
			if (loc_x == m_board_size-1) {move_to_loc = -1; break;}
			move_to_loc = (loc_x+1)+loc_y*m_board_size;
			break;
		}
		case 4:
			throw "Should not be in case 4, that is handled above";

	}

	if (move_to_loc == -1) {  //IF SNAKE OUT OF BOUNDED, KILL DONT BOTHER MOVING HIM
		m_board[ m_snake_loc[0] ] = getDeadChar();
		return false;
	}

	//BEGIN THE PROCESS OF MOVING THE SNEK

	//Reset the tile for the old head
	//Insert new head into state and tile it in board
	m_board[ m_snake_loc[0] ] = getSnekChar();
	//Really inefficient, maybe a ring data structure would be better, but it would still have to resize
	m_snake_loc.insert(m_snake_loc.begin(), move_to_loc);
	m_board[ m_snake_loc[0] ] = getHeadChar();


	//LOGIC TO CHECK IF THE HEAD HAS LANDED ON THE SAME TILE AS THE FOOD
	if (m_food_loc == move_to_loc) {
		m_food_loc = getRandomNumber( 0, m_board_size*m_board_size );
		//This means it cannot spawn in the head, but it can in the body,
		//	but I don't have a straight forward solution
		//Note, if it spawns on the body, it covers the body
		if (m_food_loc==m_snake_loc[0]){   //CHECK IF FOOD HAS SPAWNED ON HEAD
			m_food_loc = m_food_loc + 1;   //IF SO, MOVE IT OVER ONE
		}
		//ugly, but it should only happen rarely
		//except for small boards, but its too late at night to figure out a fix
		//IF THE FOOD HAS SPAWNED IN THE SAME CORNER THAT THE ENEMIES SPAWN IN MOVE IT OUT
		//THE ENEMIES GET A FREE MOVE, SO OTHERWISE THEY WILL SPAWN CAMP
		if (m_food_loc == 0 || m_food_loc == 1 || m_food_loc == m_board_size+1 ) m_food_loc = 2*m_board_size;

		m_board[ m_food_loc ] = getFoodChar();  //TILE THE NEW LOCATION
												//THE OLD LOCATION DOES NOT NEED TO BE DETILED AS THE HEAD HAS THAT TILE NOW

		m_enem_loc.push_back(0);   //ADD A NEW ENEMY AND TILE IT
		m_board[ 0 ] = getEnemChar();  //THEY GET THEIR FREE MOVE A FEW LINES DOWN IN CODE

	}
	else {  //IF THE SNEK DID NOT EAT CUT THE TAIL OFF
		if (m_snake_loc.back() != m_snake_loc.front())
			m_board[ m_snake_loc.back() ] = getBlankChar();
		m_snake_loc.pop_back();
	}

	//IF THE SNAKE IS NOT ONTOP OF ITSELF, IT HAS DIED
	//Skips the first element for obvious reasons
	if (std::find(m_snake_loc.begin()+1, m_snake_loc.end(), move_to_loc) != m_snake_loc.end())
	{
		m_board[ m_snake_loc[0] ] = getDeadChar();
		return false;
	}

	//NOW THE EMEMIES GET TO MOVE

	//Should make math for moving in the board a separate function
	//might do it later, am tired
	int emove_to_loc{};   //I KNOW THIS NAMING SCHEME IS BAD
	int eloc_x;
	int eloc_y;
	for (int i {0}; i < static_cast<int>(m_enem_loc.size()); ++i) {  //MOVE EACH ENEMY
		//There has to be a more efficient way to do this, but am tired
		//ALL ENEMIES HAVE TO MOVE PER TICK, SO PREEMPTIVELY SET THEIR TILES BACK
		if (std::find(m_snake_loc.begin(), m_snake_loc.end(), m_enem_loc[i]) != m_snake_loc.end())
			m_board[ m_enem_loc[i] ] = getSnekChar();
		else if (m_food_loc == m_enem_loc[i])
			m_board[ m_enem_loc[i] ] = getFoodChar();
		else
			m_board[ m_enem_loc[i] ] = getBlankChar();

		if (m_snake_loc[0] == m_enem_loc[i])
			m_board[ m_enem_loc[i] ] = getHeadChar();

		//BREAK DOWN THE FLATTENED COORDINATES INTO X AND Y
		eloc_x = m_enem_loc[i]%m_board_size;
		eloc_y = m_enem_loc[i]/m_board_size;
		int edir = getRandomNumber(0,3);  //GET A RANDOM DIRECTION TO MOVE IN
		//IF SAID RANDOM DIRECTION WOULD LEAD TO OUT OF BOUNDS
		//JUST FLIP IT, THE RANDOMNESS OF WALK THEN BECOMES WEIGHTED
		//BUT THAT SHOULD NOT MATTER IN THIS APPLICATION
		if (eloc_y == 0 && edir == 0) edir = 1;
		if (eloc_y == (m_board_size-1) && edir == 1) edir = 0;
		if (eloc_x == 0 && edir == 2) edir = 3;
		if (eloc_x == (m_board_size-1) && edir == 3) edir = 2;

		//Do the actual move
		if (edir == 0) eloc_y -= 1;
		if (edir == 1) eloc_y += 1;
		if (edir == 2) eloc_x -= 1;
		if (edir == 3) eloc_x += 1;  //SET THE CHANGES TO THE COORDINATES THEN REFLATTEN AND TILE
		emove_to_loc = eloc_x+eloc_y*m_board_size;
		m_enem_loc[i] = emove_to_loc;
		m_board[ m_enem_loc[i]] = getEnemChar();

		//FINALLY, CHECK IF THE ENEMIES STUMBLED UP SNEK
		//IF SO THEY KILL IT
		if (std::find(m_snake_loc.begin(), m_snake_loc.end(), emove_to_loc) != m_snake_loc.end())
		{  //I really don't like this nested loop situation
			//I don't want a O(n^2) situation, will try to figure out a fancy later
			m_board[ emove_to_loc ] = getKillChar();  //DIFFERENT TILE FOR ENEMY KILLS
			return false;
		}
	}

	m_head_direction = head_direction;  //SET HEAD DIRECTION FOR NEXT TIME'S LAZY INPUT
	return true;
}

void Snake::PrintBoard(){  //JUST PRINTS THE TILES IN m_board
							//BUT WITH A FANCY EDGECHAR BOARDER
	for (int i {0}; i < m_board_size; ++i) std::cout << ' ' <<getEdgeChar();
	std::cout << std::endl;
	std::cout << getEdgeChar();
	for (int i {0}; i < m_board_size*m_board_size; ++i) {
		if (i%m_board_size==0 && i!=0) {
			std::cout << getEdgeChar();
			std::cout << std::endl;
			std::cout << getEdgeChar();
		}
		std::cout << m_board[i] << ' ';

	}
	std::cout << getEdgeChar();
	std::cout << std::endl;
	for (int i {0}; i < m_board_size; ++i) std::cout << ' ' <<getEdgeChar();
	std::cout << std::endl;
}

Snake::~Snake() {
	delete m_board;  //deallocate,  is only dynamic
}

