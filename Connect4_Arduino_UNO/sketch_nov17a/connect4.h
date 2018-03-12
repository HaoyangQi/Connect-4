#ifndef __CONNECT_4__
#define __CONNECT_4__

#include <stdbool.h>
#include <stdio.h>
//#undef random
#include <Arduino.h>

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
#define MaxY 3

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
  short Scene[MaxY+1][MaxX+1];
  short NextMove[MaxX+1][2];
  PLAYER CurrentPlayer;
  short Moves;
}RoundState;

//extern int PossibleScenes;
extern short VictoryProbability[MaxX+1];

void DisplayScene(RoundState state);
void MakeMove(RoundState *state, short x, short y);
void RetractMove(RoundState *state, short x, short y);
short FindWinner(RoundState state);
short CalculateCoordinateY(RoundState state, short x);
bool CheckNextStep(RoundState state, short x, short y);
void GameInit(RoundState *state, PLAYER player);
short DummyPlayer(RoundState *state);
short DetermineBestMove(RoundState state, short *MoveRating);
//void Guidance();
//int ModeHelper();
//bool WaitForYesNo(char* hint);
char getch();
short GameMain();

#endif
