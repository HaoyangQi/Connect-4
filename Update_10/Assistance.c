/* Connect 4 -- External Assistance
 *
 *Those functions assist the game framework 
 *and algorithms to modify data or output 
 *something additionally to screen.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

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

/* DisplayScene()
 *
 *Display the board on the screen.
*/

void DisplayScene(RoundState state)
{
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
				printf("|   ");
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
