/*Connect 4 -- Main Loop
 *
 *This file contains the main loop of the game.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

int main()
{
	RoundState game;
	int x,y,rating;
	
	int i;
	
	GameInit(&game, PLAYER_B);
	DebugScene(game);
	
	while(FindWinner(game) == -1)
	{
		switch(game.CurrentPlayer)
		{
			case PLAYER_A:
			{
				while(1)
				{
					printf("Your move: \n");
					scanf("%d %d",&x,&y);
					if(CheckNextStep(game, x, y))
					{
						break;
					}
					printf("Illegal Input, try again.\n");
				}
				break;
			}
			
			case PLAYER_B:
				printf("Computer is thinking...");
				x = DetermineBestMove(game, &rating);
				y = CalculateCoordinateY(game, x);
				printf("\nIt makes the move (%d, %d) (%d)\n",x,y,rating);
				//printf("It simulates %d possible scenes.\n",PossibleScenes);
				//printf("VP: ");
				//for(i=0;i<=MaxX;i++)
				//{
				//	printf("%d  ",VictoryProbability[i]);
				//}
				//PossibleScenes = 0;
				//for(i=0;i<=MaxX;i++);
				//{
				//	VictoryProbability[i] = 0;
				//}
				break;
		}
		
		MakeMove(&game, x, y);
		
		DebugScene(game);
	}
	
	switch(FindWinner(game))
	{
		case -1:
			printf("Tie.");
			break;
			
		case 1:
			printf("You win.");
			break;
			
		case 2:
			printf("You lose.");
			break;
	}
	
	return 0;
}
