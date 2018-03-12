/*Connect 4 -- Fundamental Framework
 *
 *This file contains core functions of this game.
 *
 *Game Initializer     - GameInit()
 *Process Control      - Opponent()
 *Rules Implementation - CheckNextStep() and FindWinner()
 *Interactions         - MakeMove() and RetractMove()
 *Other                - CalculateCoordinateY() and DebugScene()
 */

#include <stdio.h>
#include <stdbool.h>

#include "conpriv.h"

void GameInit(RoundState *state, PLAYER player)
{
	// For all arrays, not like the way in declarations, they should be 
	//initialized dynamically.
	
	int i,j;
	
	// Initialize Scene Matrix
	for(i=0;i<=MaxY;i++)
	{
		for(j=0;j<=MaxX;j++)
		{
			state->Scene[i][j] = -1;
		}
	}
	// Modify the bottom line
	for(j=0;j<=MaxX;j++)
	{
		state->Scene[MaxY][j] = 0;
	}
	
	// Initialize NextMove Matrix
	for(i=0;i<=MaxX;i++)
	{
		state->NextMove[i][0] = i;
		state->NextMove[i][1] = MaxY;
	}
	
	state->CurrentPlayer = player;
	
	// Reset the global counter
	state->Moves = 0;
	
	// Initialize the seed
	srand((unsigned int)time(NULL));
}

/* Opponent()
 *
 *UPDATE: Find the opponent of the player who is currently playing.
*/

PLAYER Opponent(PLAYER CurrentPlayer)
{
	switch(CurrentPlayer)
	{
		case PLAYER_A:
			return PLAYER_B;
			
		case PLAYER_B:
			return PLAYER_A;
	}
}

/* CheckNextStep()
 *
 *UPDATE: For the consideration of efficiency, this function now 
 *only works for users in the main loop.
*/

bool CheckNextStep(RoundState state, int x, int y)
{
	int i = 0;
	for(i=0;i<=MaxX;i++)
	{
		if(state.NextMove[i][0] == -1)
		{
			continue;
		}
		else if(state.NextMove[i][0] == x && state.NextMove[i][1] == y)
		{
			return true;
		}
	}
	
	return false;
}

/* MakeMove()
 *Old SetNewChess()
 *
 *UPDATE: In order to consider the effeciency, MakeMove() 
 *will no longer check if the coordinate is in the correct range.
*/

void MakeMove(RoundState *state, int x, int y)
{
	// Mark that position
	state->Scene[y][x] = state->CurrentPlayer;
	
	// Update the NextMove Matrix
	if(y > 0)
	{
		state->Scene[y-1][x] = 0;
		state->NextMove[x][0] = x;
		state->NextMove[x][1] = y-1;
	}
	// If we reach the top, disable this column
	else if(y == 0)
	{
		state->NextMove[x][0] = -1;
		state->NextMove[x][1] = -1;
	}
	
	// Reverse the player
	state->CurrentPlayer = Opponent(state->CurrentPlayer);
	
	// Finish this turn
	state->Moves++;
}

/* RetractMove()
 *
 *UPDATE: Retract one move, actually does the same job as MakeMove().
 *
 *UPDATE: In order to consider the effeciency, RetractMove() 
 *will also not check if the coordinate is in the correct range. 
 *So if it will be used by users, adding a function CheckRetraction() 
 *is necessary to make sure if this position is the top number(1 or 2) 
 *of this column.
*/

void RetractMove(RoundState *state, int x, int y)
{
	// Reset this position
	state->Scene[y][x] = 0;
	// If there is any meaningless zero, reset it
	if(y > 0)
	{
		state->Scene[y-1][x] = -1;
	}
	
	// Update the NextMove Matrix
	// We do not need to check the y-coordinate now, 
	//because you cannot make a move outside anyway.
	state->NextMove[x][0] = x;
	state->NextMove[x][1] = y;
	
	// Reverse the player
	state->CurrentPlayer = Opponent(state->CurrentPlayer);
	
	// Roll back this turn
	state->Moves--;
}

/* FindWinner()
 *
 *UPDATE: new update of CheckWinner(), it is faster.
*/

int FindWinner(RoundState state)
{
	int x,y,m,n;
	int NextX,NextY;
	int id;
	bool pass;
	
	if(state.Moves < 7)
	{
		return -1;
	}
	
	for(x=0;x<=MaxX;x++)
	{
		for(y=MaxY;y>=0;y--)
		{
			id = state.Scene[y][x];
			
			if(id == 0 || id == -1)
			{
				//Instead of 'continue', we search the Scene vertically, 
				//based on the rule, it would be meaningless if we linear 
				//search a winner strand based on a reference of 0 or -1.
				//Surprisingly, the values above 0 would always be -1.
				break;
			}
			
			for(m=0;m<8;m++) // Direction
			{
				pass = true;
				NextX = x;
				NextY = y;
				
				for(n=0;n<3;n++) // Number of points scanned forward
				{
					NextX += Direction[m][0];
					NextY += Direction[m][1];
					
					if((NextX<0 || NextX>MaxX) || (NextY<0 || NextY>MaxY))
					{
						//This strand is a dead strand, so it is not 
						//necessary to step forward along this direction.
						pass = false;
						break;
					}
					
					if(state.Scene[NextY][NextX] != id)
					{
						//This strand is a dead strand, so it is not 
						//necessary to step forward along this direction.
						pass = false;
						break;
					}
				}
				
				if(pass) // All three points are passed
				{
					// Winner strand is detected, no need to continue on.
					return id;
				}
			}
		}
	}
	
	return -1;
}

/* CalculateCoordinateY()
 *
 *This function is a helper function of DetermineBestMove().
 *
 *DetermineBestMove() only returns the x-coordinate of next 
 *move. So CalculateCoordinateY() can scan NextMove Matrix 
 *to find the y-coordinate. Because in each turn, the 
 *x-coordinate of next move is unique.
*/

int CalculateCoordinateY(RoundState state, int x)
{
	int i;
	
	for(i=0;i<=MaxX;i++)
	{
		if(state.NextMove[i][0] == x)
		{
			return state.NextMove[i][1];
		}
	}
	
	return -1;
}

void DebugScene(RoundState state)
{
	printf("\n\nCurrent Scene Matrix:\n");
	
	int i,j;
	int k,code;
	
	for(k=0;k<=MaxX;k++)
	{
		printf("   %d",k);
	}
	
	printf("\n");
	
	for(i=0;i<=MaxY;i++)
	{
		printf(" ");
		for(k=0;k<=MaxX;k++)
		{
			printf("+---");
		}
		printf("+\n");
		printf("%d",i);
		
		for(j=0;j<=MaxX;j++)
		{
			code = state.Scene[i][j];
			
			if(code == -1)
			{
				printf("| x ");
			}
			else
			{
				printf("| %d ", state.Scene[i][j]);
			}
		}
		printf("|\n");
	}
	
	printf(" ");
	for(k=0;k<=MaxX;k++)
	{
		printf("+---");
	}
	printf("+\n");
	for(k=0;k<=MaxX;k++)
	{
		printf("   %d",k);
	}
	
	printf("\n\n");
}
