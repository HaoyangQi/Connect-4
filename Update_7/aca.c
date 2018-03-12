/* Connect 4 -- Advanced Competing Algorithm (ACA)
 *
 *The ACA is the Hard Mode implementation based on Minimax Algorithm.
 *
 *Evaluation      - EvaluateBestMove() and EvaluatePosition()
 *External Packer - DetermineBestMove()
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "conpriv.h"

/*Rating System -- Primary Evaluation
 *
 *Minimax Algorithm will rate moves as the 
 *reference of the primary evaluation. More 
 *meaningful values are WinPosition and 
 *LosePosition.
 *
 *Because the algorithm evaluates on the view 
 *of computer, WinPosition would be the best; 
 *since NeutralPosition is relatively 
 *meaningless, so it is set to the lowest.
*/

#define WinPosition     1000
#define LosePosition    0
#define NeutralPosition -1000

/* MaxDepth
*/

#define MaxDepth 8

/* Rating System -- Secondary Evaluation
 *
 *It will be used only when the Primary Evaluation 
 *NeutralPosition. The thorough explanation of 
 *it is stated at DetermineBestMove().
*/

int VictoryProbability[MaxX+1] = {0};

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
 *Since it is a game, the winning and losing scenes might occur at 
 *any round, instead after all blocks are filled. So the number of 
 *all possiblities would be extremely hard to determine accurately.
*/

int EvaluatePosition(RoundState, int);

/* EvaluateBestMove()
 *
 *Entry point of the evaluation.
 *
 *Find a best move among those simulated scenes. how many final-round 
 *situations can be simulated depends on depth value.
 *
 *The concept is easy: EvaluateBestMove(), assisted with EvaluatePosition(), 
 *continuously plays(simulates) this game. When a result occurs(win/lose/draw), 
 *EvaluateBestMove() will catch a rating of this simulation, if the rating is 
 *benefit for computer/not benefit for user, and better than previous one, this 
 *simulation(move) will be reserved.
*/

int EvaluateBestMove(RoundState state, int *MoveRating, int depth)
{
	int i;
	int Move[2], BestMoveX;
	int MaxRating = NeutralPosition - 1; // in order to be replaced at the first time
	int Rating;
	
	for(i=0; i<=MaxX; i++)
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
		
		// Secondary Rating Mechanism
		if(Rating == WinPosition)
		{
			VictoryProbability[i]++;
		}
		
		// Primary Rating Mechanism
		if (Rating > MaxRating)
		{
			BestMoveX = Move[0];
			MaxRating = Rating;
        }
        
		// Retract this move, that is why we call it 'virtual'
		RetractMove(&state, Move[0], Move[1]);
	}
	
	// Rating of the CURRENT Move
	//This is tricky: Each simulated step is actually made by different 
	//players. Since the rating is used for computer (PLAYER_B), so a 
	//good move for PLAYER_B is a bad move for PLAYER_A. Therefore when 
	//the recursion is rolling back, we need to reverse the evaluation 
	//result if the current player is switched to PLAYER_A.
	//Which means that this algorithm is actually evaluating for both sides, 
	//but only the best move for computer (PLAYER_B) should be preserved and 
	//the rest should be eliminated properly.
	*MoveRating = (state.CurrentPlayer == PLAYER_B)?(MaxRating):(-MaxRating);
	
	/*
	if(*MoveRating == NeutralPosition)
	{
		BestMoveX = FindMaxVP();
	}
	*/
	
	//For this game only, we do not need to return a coordinate. 
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
		//PossibleScenes++;
		
		if(code == PLAYER_B)
		{
			return WinPosition;
		}
		else if(code == PLAYER_A)
		{
			return LosePosition;
		}
		else
		{
			return NeutralPosition;
		}
	}
	
	// Game is undergoing, proceed the simulation
	EvaluateBestMove(state, &rate, depth);

	return rate;
}

/* DetermineBestMove()
 *
 *It is an external packer function to make a final 
 *decision through considering ratings from primary 
 *and secondary rating system.
 *
 *The concept is: because of the limitation of depth 
 *value, many scenes might not reveal a result. This 
 *time the primary evaluation will always return 
 *NeutralPosition. Then we need the secondary evaluation. 
 *
 *VictoryProbability stores the total number of scenes of 
 *WinPosition for each point where you can make the move.
 *A higher number stands for a higher chance you are going 
 *to win finally.
*/

int DetermineBestMove(RoundState state, int *MoveRating)
{
	int BestX;
	int i, max = -1;
	
	BestX = EvaluateBestMove(state, MoveRating, 0);
	
	if(*MoveRating == NeutralPosition)
	{
		// Find the maximum probability
		for(i=0;i<=MaxX;i++)
		{
			if(VictoryProbability[i] > max)
			{
				max = VictoryProbability[i];
				BestX = i;
			}
		}
	}
	
	// Clean up VictoryProbability for the next round.
	for(i=0;i<=MaxX;i++);
	{
		VictoryProbability[i] = 0;
	}
	
	return BestX;
}
