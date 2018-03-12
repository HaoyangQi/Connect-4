/* Connect 4 -- Constants, Data Structures, and Global Variables
 *
 *This file contains important data that are used to construct the 
 *basic framework.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Domain of Coordinates
 *
 *Those two constants specify the 
 *maximum value that can be applied 
 *to this game.
 *
 *The domain is:
 *Any point at (0,0) to (6,5).
 *
 *The Coordinate System is shown below.
*/

#define MaxX 6
#define MaxY 5

//Player Flag
typedef enum
{
	PLAYER_A = 1, // For User
	PLAYER_B = 2  // For Computer
}PLAYER;

/* RoundState -- Basic Construction of the Game
 *
 *Data Members:
 * Scene    - The whole board. It will be constantly updated 
 *            when the game is undergoing.
 *
 * NextMove - Because of the rules, positions that are available 
 *            for the next move are restricted: consider the board 
 *            as a top-open box, the only thing you can do is grabbing 
 *            a ball and let it free drop to the bottom.
 *
 * CurrentPlayer - Specify which player is currently playing.
 *
 * Moves    - Count the moves that have been made.
*/

/* Thorough Explanation of Scene
 *
 *A (MaxY+1) * (MaxX+1) two-dimensional board.
 *For each block: -1 means forbidden (in next move)
 *                0  means available and no chess in it
 *                1  means occupied by player A (User)
 *                2  means occupied by player B (Computer)
 *
 *  0  1  2  3  4  5  6
 * +--+--+--+--+--+--+--+
 *0|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *1|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *2|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *3|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *4|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *5|  |  |  |  |  |  |  |
 * +--+--+--+--+--+--+--+
 *
 *Initially, the bottom line should be set by 0, the other should be set by -1.
*/

typedef struct
{
	int Scene[MaxY+1][MaxX+1];
	int NextMove[MaxX+1][2];
	PLAYER CurrentPlayer;
	int Moves;
}RoundState;

//extern int PossibleScenes;
extern int VictoryProbability[MaxX+1];
