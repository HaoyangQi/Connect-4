/* Connect 4 -- Dummy Player (Easy Mode Implementation)
 *
 *In Easy Mode, the opponent of user will make a move randomly.
*/

#include <stdio.h>
#include <stdbool.h>

#include "conpriv.h"

/* RandCreate()
 *
 *Create a random number within the domain [low, high).
*/

int RandCreate(int low, int high) 
{
	int val;
	val = ((int)(rand()%(high - low)) + low);
	return(val);
}

/* DummyPlayer()
 *
 *A dummy procedure to play against players.
 *
 *Dummy player uses a randomly picked coordinate to make a move.
*/

void DummyPlayer(RoundState *state)
{
	int choice;
	
	while(1)
	{
		// Why MaxX+1? See comments of RandCreate() and RoundState
		choice = RandCreate(0, MaxX+1);
		if(state->NextMove[choice][0] != -1)
		{
			break;
		}
	}
	
	MakeMove(state, state->NextMove[choice][0], state->NextMove[choice][1]);
}
