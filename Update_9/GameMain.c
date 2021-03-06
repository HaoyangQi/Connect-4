/*Connect 4 -- Main Loop
 *
 *This file contains the main loop of the game.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

void GameMain_EasyMode(RoundState *state)
{
	int x,y,rating;
	
	DisplayScene(*state);
	
	while((FindWinner(*state) == -1) && (state->Moves <= (MaxX+1)*(MaxY+1)))
	{
		switch(state->CurrentPlayer)
		{
			case PLAYER_A:
			{
				while(1)
				{
					printf("Your move: ");
					//scanf("%d %d",&x,&y);
					scanf("%d",&x);
					y = CalculateCoordinateY(*state, x);
					if(CheckNextStep(*state, x, y))
					{
						break;
					}
					printf("Illegal Input, try again.\n");
				}
				system("cls");
				break;
			}
			
			case PLAYER_B:
			{
				x = DummyPlayer(state);
				y = CalculateCoordinateY(*state, x);
				system("cls");
				printf("Computer makes a move (%d,%d).\n",x,y);
				break;
			}
		}
		
		MakeMove(state, x, y);
		
		DisplayScene(*state);
	}
	
	return;
}

void GameMain_HardMode(RoundState *state)
{
	int x,y,rating;
	
	DisplayScene(*state);
	
	while((FindWinner(*state) == -1) && (state->Moves <= (MaxX+1)*(MaxY+1)))
	{
		switch(state->CurrentPlayer)
		{
			case PLAYER_A:
			{
				while(1)
				{
					printf("Your move: ");
					//scanf("%d %d",&x,&y);
					scanf("%d",&x);
					y = CalculateCoordinateY(*state, x);
					if(CheckNextStep(*state, x, y))
					{
						break;
					}
					printf("Illegal Input, try again.\n");
				}
				system("cls");
				break;
			}
			
			case PLAYER_B:
				printf("Computer is thinking...");
				x = DetermineBestMove(*state, &rating);
				y = CalculateCoordinateY(*state, x);
				system("cls");
				printf("\nIt makes the move (%d, %d) (%d)\n",x,y,rating);
				break;
		}
		
		MakeMove(state, x, y);
		
		DisplayScene(*state);
	}
	
	return;
}

void GameMain_HellMode(RoundState *state)
{
	int x,y,rating;
	
	//DisplayScene(*state);
	
	while((FindWinner(*state) == -1) && (state->Moves <= (MaxX+1)*(MaxY+1)))
	{
		switch(state->CurrentPlayer)
		{
			case PLAYER_A:
			{
				while(1)
				{
					printf("Your move: ");
					//scanf("%d %d",&x,&y);
					scanf("%d",&x);
					y = CalculateCoordinateY(*state, x);
					if(CheckNextStep(*state, x, y))
					{
						break;
					}
					printf("Illegal Input, try again.\n");
				}
				//system("cls");
				break;
			}
			
			case PLAYER_B:
				printf("Computer is thinking...");
				x = DetermineBestMove(*state, &rating);
				y = CalculateCoordinateY(*state, x);
				//system("cls");
				printf("\nIt makes the move (%d, %d) (%d)\n",x,y,rating);
				break;
		}
		
		MakeMove(state, x, y);
		
		//DisplayScene(*state);
	}
	
	return;
}

void GameMain_TwoPlayerMode(RoundState *state)
{
	int x,y,rating;
	
	DisplayScene(*state);
	
	while((FindWinner(*state) == -1) && (state->Moves <= (MaxX+1)*(MaxY+1)))
	{
		printf("%s makes a move: ", state->CurrentPlayer==PLAYER_A?"Player A":"Player B");
		
		while(1)
		{
			scanf("%d",&x);
			y = CalculateCoordinateY(*state, x);
			if(CheckNextStep(*state, x, y))
			{
				break;
			}
			printf("Illegal Input, try again.\n");
		}
		
		system("cls");
		printf("%s makes a move (%d,%d).",
		       state->CurrentPlayer==PLAYER_A?"Player A":"Player B",
			   x,y);
		
		MakeMove(state, x, y);
		
		DisplayScene(*state);
	}
	
	return;
}

int main()
{
	RoundState game;
	//int x,y,rating;
	//int i;
	
	int choice = Guidance();
	
	GameInit(&game, PLAYER_B);
	
	switch(choice)
	{
		case 1:
			GameMain_EasyMode(&game);
			break;
			
		case 2:
			GameMain_HardMode(&game);
			break;
			
		case 3:
			GameMain_HellMode(&game);
			break;
			
		case 4:
			GameMain_TwoPlayerMode(&game);
			break;
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
