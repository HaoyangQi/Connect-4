#ifndef _CONPRIV_H_
#define _CONPRIV_H_

#include <stdio.h>
#include <stdbool.h>

#define MaxX 6
#define MaxY 5

/* Grading System*/
#define WinPosition     1000
#define NeutralPosition 0
#define LosePosition    (-WinPosition)

#define MaxDepth        7

extern int PossibleScenes;

// UPDATE: Old player constants
typedef enum
{
	PLAYER_A = 1, // For User
	PLAYER_B = 2  // For Computer
}PLAYER;

typedef struct
{
	int Scene[MaxY+1][MaxX+1];
	int NextMove[MaxX+1][2]; // UPDATE: Old Update Matrix
	PLAYER CurrentPlayer;    // Who is currently playing.
	int Moves;               // UPDATE: Old global counter
}RoundState;

/* Direction Matrix
 *
 *Used by FindWinner().
 *
 *Each coordinate represents a direction vector. The whole matrix 
 *states 8 directions relative to a center point.
*/

extern const int Direction[8][2];

extern int VictoryProbability[MaxX+1];

#endif
