#ifndef MAIN_H
#define MAIN_H

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

// Player
typedef struct player {
    int row;
    int col;
    int lives;
} Player;

// Enemy

typedef struct enemy {
	int row;
	int col;
	int headRow;
} Enemy;

#endif
