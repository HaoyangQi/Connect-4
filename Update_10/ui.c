/* Connect 4 -- User Interface Implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

const char* Instruction1 = 
"\nWelcome to Connect 4\n\n"
"The following instructions will teach you how to play this game.\n\n"
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
"Good luck and enjoy this game :)\n\n";

/* WaitForSpace and WaitForYesNo
 *
 *Get a restricted input from user for pauses.
*/

void WaitForSpace(char* hint)
{
	char ch;
	
	printf(hint);
	
	while(1)
	{
		ch = getch();
		
		if(ch == ' ')
		{
			system("cls");
			break;
		}
	}
}

bool WaitForYesNo(char* hint)
{
	char ch;
	bool bRet;
	
	printf(hint);
	
	while(1)
	{
		ch = getch();
		
		if(ch == 'y' || ch == 'Y')
		{
			bRet = true;
			break;
		}
		
		if(ch == 'n' || ch == 'N')
		{
			bRet = false;
			break;
		}
	}
	
	return bRet;
}

/* DemoHelper
 *
 *A special move-making procedure, for simulating 
 *a real game scenario.
 *
 *In order to use this function properly, the member 
 *Moves in RoundState must be adjusted to 7 (or a value 
 *that is slightly higher that 7) to ensure FindWinner 
 *runs properly (for more references, see EnterInstruction).
 *
 *The reason is that in the demo procedure, '2 players' is 
 *not meaningful. So for each turn, the player will be 
 *locked at the same one (PLAYER_A or PLAYER_B). For more 
 *references, see FindWinner and MakeMove.
*/

void DemoHelper(RoundState *game)
{
	int x,y;
	
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
}

/* EnterInstruction()
 *
 *Guidence of instructions.
*/

void EnterInstruction(RoundState *game)
{
	system("cls");
	
	printf(Instruction1, MaxY+1, MaxX+1);
	
	DisplayScene(*game);
	
	WaitForSpace("\n\nPress SPACE to proceed.");
	
	printf(Instruction2, MaxX);
	
	DisplayScene(*game);
	
	// Hack the move counter, because FindWinner() works only after the 7th move.
	game->Moves = 7;
	
	DemoHelper(game);
	
	printf(Instruction3);
	
	DisplayScene(*game);
	
	while(FindWinner(*game) == -1)
	{
		DemoHelper(game);
		DisplayScene(*game);
	}
	
	printf(Instruction4);
	
	WaitForSpace("\n\nPress SPACE to finish.");
}

/* ModeHelper()
 *
 *Guidence of game modes.
 *
 *Easy Mode
 *	Computer randomly makes a move.
 *
 *Hard Mode
 *	Computer carefully makes a move.
 *
 *Hell Mode
 *	Computer carefully makes a move, meanwhile, 
 *users will not see any board update on the screen, 
 *but coordinates.
 *
 *2-player mode
 *	2 users play the game.
*/

int ModeHelper()
{
	char ch;
	
	printf("\n\nChoose a mode to play:\n"
	       "1 -> Easy mode\n"
	       "2 -> Hard mode\n"
	       "3 -> Hell mode\n"
	       "4 -> 2-player mode\n"
	       "Enter your choice(1|2|3|4):");
	
	while(1)
	{
		ch = getch();
		
		switch(ch)
		{
			case '1':
				return 1;
			
			case '2':
				return 2;
			
			case '3':
				return 3;
				
			case '4':
				return 4;
			
			default:
				break;
		}
	}
}

/* Guidance()
 *
 *A run-once function that satrts when this program just starts.
*/

void Guidance()
{
	RoundState game;
	bool bRules;
	
	system("cls");
	
	printf("Welcome to Connect 4\n\n");
	
	bRules = WaitForYesNo("Would you like to know more about the rules?(y/n)");
	
	if(bRules)
	{
		GameInit(&game,PLAYER_A);
		EnterInstruction(&game);
	}
}
