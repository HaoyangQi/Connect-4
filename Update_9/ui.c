/* Connect 4 -- User Interface Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

const char* Instruction1 = 
"\nWelcome to Connect 4\n\n"
"The following instructions will teach you how to play this game.\n"
"First, There is a %d * %d board.\n";

const char* Instruction2 = 
"You can see numbers along edges, when you make a move, you only "
"need to choose one number from those shown on the top or the bottom.\n"
"In this case, your input should be 0~%d.\n"
"Because where you make the next move is restricted. Those blocks that "
"contains '0' are open for the next move.\n"
"Give it a try, make a move.\n\n";

const char* Instruction3 = 
"Nicely done! Now you can play, but before you get started. You have to "
"know how to win in this game.\n"
"Just like what the name tells you, you need to connect at least 4 chess "
"of yours along one of eight directions to win.\n"
"Now, try to connect 4 chess.\n\n";

const char* Instruction4 = 
"Congratulations, you win!\n"
"This is just a demo procedure, of course. When you start a new game, you and "
"another player or computer take turns to make moves.\n"
"For the record, When you play with computer, '1' stands for you, '2' stands for "
"the computer.\n\n"
"Good luck and enjoy this game :)\n\n"
"Now you can press any key to exit this instruction.";

void EnterInstruction(RoundState *game)
{
	int x,y;
	
	system("cls");
	
	printf(Instruction1, MaxY+1, MaxX+1);
	
	DisplayScene(*game);
	
	printf(Instruction2, MaxX);
	
	DisplayScene(*game);
	
	// Hack the move counter, because FindWinner() works only after the 7th move.
	game->Moves = 7;
	
	while(1)
	{
		printf("Your move: \n");
		scanf("%d",&x);
		y = CalculateCoordinateY(*game, x);
		if(CheckNextStep(*game, x, y))
		{
			break;
		}
		printf("Illegal Input, try again.\n");
	}
	MakeMove(game, x, y);
	// Lock the player, it is just a demo.
	game->CurrentPlayer = PLAYER_A;
	
	printf(Instruction3);
	
	DisplayScene(*game);
	
	while(FindWinner(*game) == -1)
	{
		while(1)
		{
			printf("Your move: ");
			scanf("%d",&x);
			y = CalculateCoordinateY(*game, x);
			if(CheckNextStep(*game, x, y))
			{
				break;
			}
			printf("Illegal Input, try again.\n");
		}
		
		MakeMove(game, x, y);
		// Lock the player, it is just a demo.
		game->CurrentPlayer = PLAYER_A;
		DisplayScene(*game);
	}
	
	printf(Instruction4);
	
	getch();
	system("cls");
}

int Guidance()
{
	RoundState game;
	bool bRules;
	char ch;
	
	GameInit(&game,PLAYER_A);
	
	system("cls");
	
	printf("Welcome to Connect 4\n\n"
	       "Would you like to know more about the rules?(y/n)");
	
	while(1)
	{
		ch = getch();
		
		if(ch == 'y' || ch == 'Y')
		{
			bRules = true;
			break;
		}
		
		if(ch == 'n' || ch == 'N')
		{
			bRules = false;
			break;
		}
	}
	
	if(bRules)
	{
		EnterInstruction(&game);
	}
	
	printf("\nChoose a mode to play:\n"
	       "1 -> Easy mode\n"
	       "2 -> Hard mode\n"
	       "3 -> Hell mode\n"
	       "4 -> 2-player mode\n"
	       "Enter your choice:");
	
	while(1)
	{
		ch = getch();
		
		switch(ch)
		{
			case '1':
				printf("1\n\n");
				return 1;
			
			case '2':
				printf("2\n\n");
				return 2;
			
			case '3':
				printf("3\n\n");
				return 3;
				
			case '4':
				printf("4\n\n");
				return 4;
			
			default:
				break;
		}
	}
}
