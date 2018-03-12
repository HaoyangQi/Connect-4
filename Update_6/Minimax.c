/*Connect 4 -- Minimax Algorithm (Hard Mode Implementation)
 *
 *This file contains the implementation of Minimax Algorithm.
 */

#include <stdio.h>
#include <stdbool.h>

#include "conpriv.h"

int VictoryProbability[MaxX+1] = {0};

int FindMaxVP()
{
	int i, tmp=-1, x;
	
	for(i=0;i<=MaxX;i++)
	{
		if(VictoryProbability[i] > tmp)
		{
			tmp = VictoryProbability[i];
		}
	}
	
	for(i=0;i<=MaxX;i++)
	{
		if(tmp == VictoryProbability[i])
		{
			x = i;
			break; // Just in case if there are equal numbers
		}
	}
	
	return x;
}

/* Minimax Algorithm
 *
 *DetermineBestMove() and EvaluatePosition()
 *
 *The core is a recursion involved two functions. These 2 functions 
 *call each other to evaluate 'current' Scene.
 *
 *DetermineBestMove() will simulate what the Scene would look like 
 *next step as many as possible. How many scenario it can evaluate 
 *depends on the 'depth' value. Bsed on the experimental data, when 
 *depth = 10, it is goning to take more than a minute to think.
 *
 *One simpler example will answer why it costs that much time:
 *
 *For the game tic-tac-toe, there are 3x3 = 9 blocks, so the total 
 *number of all possible scenes of this game might be:
 *
 *2*!9 = 725,760
 *
 *For this game, there are 42 blocks! Based on the rules, the total 
 *number of all possible scenes of this game might be:
 *
 *2*(7^5 * !7) = 169,414,560
 *
 *For the future reference, if there are no limitations of where to 
 *set, the total number of all possible scenes of this game might be:
 *
 *!42 = 1.46120636 * 10^51
 *
 *WAIT!!!! Are those numbers right? NO! Since it is a game, the winning 
 *and losing scenes might occur at any round, instead after all blocks 
 *are filled. So actual scenes are less than the number showed above, the 
 *remainder, however, is still large.
*/

int EvaluatePosition(RoundState, int);

/* DetermineBestMove()
 *
 *Find a best move among those simulated scenes. how many final-round 
 *situations can be simulated depends on depth value.
 *
 *The concept is easy: DetermineBestMove(), associated with EvaluatePosition(), 
 *continuously plays(simulates) this game. When a result occurs(win/lose/draw), 
 *DetermineBestMove() will catch a rating of this simulation, if the rating is 
 *benefit for computer/not benefit for user, and better than previous one, this 
 *simulation(move) will be reserved.
*/

int DetermineBestMove(RoundState state, int *MoveRating, int depth)
{
	int i;
	int Move[2], BestMoveX;
	int MinimumRating = WinPosition + 1; // in order to be replaced at the first time
	//int MaxRating = LosePosition - 1; // in order to be replaced at the first time
	int Rating;
	
	for(i=0; i<=MaxX && MinimumRating != LosePosition; i++)
	//for(i=0; i<=MaxX && MaxRating != WinPosition; i++)
	{
		if(state.NextMove[i][0] == -1)
		{
			continue;
		}
		
		Move[0] = state.NextMove[i][0];
		Move[1] = state.NextMove[i][1];
		
		// Tactical Prediction Stage
		
		// Virtually make a move
		MakeMove(&state, Move[0], Move[1]);
		
		// Evaluate this move
		Rating = EvaluatePosition(state, depth + 1);
		
		if(Rating == LosePosition && depth + 1 == MaxDepth)//WinPosition)
		{
			VictoryProbability[i]++;
		}
		
		if (Rating < MinimumRating)
		{
			BestMoveX = Move[0];
			MinimumRating = Rating;
        }
		/*
		if (Rating > MaxRating)
		{
			BestMoveX = Move[0];
			MaxRating = Rating;
        }
        */
		// Retract this move, that is why we call it 'virtual'
		RetractMove(&state, Move[0], Move[1]);
	}
	
	*MoveRating = -MinimumRating; // Rating of the Round
	//*MoveRating = -MaxRating; // Rating of the Round
	
	if(*MoveRating == 0)
	{
		BestMoveX = FindMaxVP();
	}
	
	// For this game only, we do not need to return a coordinate. 
	//Because at each turn, the x-coordinate is unique in NextMove, 
	//y-coordinate, however, is not.
	return BestMoveX;
}

/* EvaluatePosition()
 *
 *This function is easier to understand: check if the simulation is 
 *over(hence the game is over). If so, grade this simulation; if not, 
 *come back to DetermineBestMove() to proceed the current simulation.
*/

int EvaluatePosition(RoundState state, int depth)
{
	int rate;
	int code = FindWinner(state);
	
	if(code != -1 || depth >= MaxDepth)
	{
		PossibleScenes++;
		
		if(code == state.CurrentPlayer)//PLAYER_B)
		{
			return WinPosition;
		}
		else if(code == Opponent(state.CurrentPlayer))//PLAYER_A)
		{
			return LosePosition;
		}
		else
		{
			return NeutralPosition;
		}
	}
	
	DetermineBestMove(state, &rate, depth); // Game is undergoing, proceed the simulation
	
	return rate;
}
