#include "connect4.h"

/* CalculateCoordinateY()
 *
 *This function is a helper function of DetermineBestMove().
 *
 *DetermineBestMove() only returns the x-coordinate of next 
 *move. So CalculateCoordinateY() can scan NextMove Matrix 
 *to find the y-coordinate. Because in each turn, the 
 *x-coordinate of next move is unique.
*/

short CalculateCoordinateY(RoundState state, short x)
{
  short i;
  
  for(i=0;i<=MaxX;i++)
  {
    if(state.NextMove[i][0] == x)
    {
      return state.NextMove[i][1];
    }
  }
  
  return -1;
}

void DisplayScene(RoundState state)
{
  short i,j;
  short k,code;
  
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

/* Direction Matrix
 *
 *Used by FindWinner().
 *
 *Each coordinate represents a direction vector. The whole matrix 
 *states 8 directions relative to a center point.
*/

const short Direction[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

/* GameInit()
 *
 *Preparations before the game starts.
*/

void GameInit(RoundState *state, PLAYER player)
{
  short i,j;
  
  for(i=0;i<=MaxY;i++)
  {
    for(j=0;j<=MaxX;j++)
    {
      state->Scene[i][j] = -1;
    }
  }
  
  for(j=0;j<=MaxX;j++)
  {
    state->Scene[MaxY][j] = 0;
  }
  
  for(i=0;i<=MaxX;i++)
  {
    state->NextMove[i][0] = i;
    state->NextMove[i][1] = MaxY;
  }
  
  state->CurrentPlayer = player;
  
  // Reset the global counter
  state->Moves = 0;
  
  // Initialize VictoryProbability Matrix
  for(i=0;i<=MaxX;i++)
  {
    VictoryProbability[i] = 0;
  }
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

bool CheckNextStep(RoundState state, short x, short y)
{
  short i = 0;
  
  if(y == -1)
    return false;
  
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

void MakeMove(RoundState *state, short x, short y)
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

void RetractMove(RoundState *state, short x, short y)
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

short FindWinner(RoundState state)
{
  short x,y,m,n;
  short NextX,NextY;
  short id;
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

/* GameMain_HardMode()
 *
 *The game loop for Hard Mode.
 *
 *Due to the limitation of the memory of the chip, only this mode is preserved.
*/

void GameMain_HardMode(RoundState *state)
{
  short x,y,rating;
  
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
          //scanf("%d",&x);
          x=GetDigit();
          printf("%d\n",x);
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
        printf("\nIt makes the move (%d, %d)\n",x,y);
        break;
    }
    
    MakeMove(state, x, y);
    
    DisplayScene(*state);
  }
  
  return;
}

/* main
 *
 *The entry point of the game.
*/

short GameMain()
{
  RoundState game;
  //int choice;
  
  {
    //choice = 2;//ModeHelper();
    GameInit(&game, PLAYER_B);
    
    printf("\n\n");
    
    GameMain_HardMode(&game);
    
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
  }
  
  return 0;
}

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
#define LosePosition    -1000
#define NeutralPosition 0

#define MaxDepth 4

/* Rating System -- Secondary Evaluation
 *
 *It will be used only when the Primary Evaluation 
 *NeutralPosition. The thorough explanation of 
 *it is stated at DetermineBestMove().
*/

short VictoryProbability[MaxX+1] = {0};

short EvaluatePosition(RoundState, short);

short CriticalCnt=0,CriticalPos=-1;

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
 *depth >= 5, it is goning to crash.
*/

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

short EvaluateBestMove(RoundState state, short *MoveRating, short depth)
{
  short i;
  short Move[2], BestMoveX;
  short MaxRating = LosePosition - 1; // in order to be replaced at the first time
  short Rating;
  
  for(i=0; i<=MaxX; i++)
  {
    if(state.NextMove[i][0] == -1&&depth==0)
    {
      //printf("Detected at %d\n",i);
      VictoryProbability[i]-=100;
      continue;
    }
    
    Move[0] = state.NextMove[i][0];
    Move[1] = state.NextMove[i][1];
    
    // Tactical Prediction Stage
    
    // Virtually make a move
    MakeMove(&state, Move[0], Move[1]);
    
    // Evaluate this move
    Rating = EvaluatePosition(state, depth + 1);
    //if(depth==1) printf("EVAL (%d)(%d)(%d)\n",i,Rating,MaxRating);
    // Secondary Rating Mechanism
    if(Rating == WinPosition)
    {
      VictoryProbability[i]++;
    }
    //if(Rating == LosePosition)
    //{
      //VictoryProbability[i]--;
    //}
    // If next step is critical, do this first
    if(Rating == LosePosition && depth==1)
    {
      //printf("CRITICAL (%d)(%d)(%d)\n",i,Rating,MaxRating);
      CriticalCnt++;
      CriticalPos=i;
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
  //if(depth==0&&CriticalPos!=-1) printf("\n%d %d\n",CriticalCnt,CriticalPos);
  //Compuer will definitely lose next step without making this step.
  if(CriticalCnt==MaxX && depth==0)
  {
    *MoveRating = WinPosition; // Hack the move rating
    BestMoveX = CriticalPos;
    CriticalCnt=0;
    CriticalPos=-1;
  }
  return BestMoveX;
}

/* EvaluatePosition()
 *
 *This function is easier to understand: check if the simulation is 
 *over(hence the game is over). If so, grade this simulation; if not, 
 *come back to DetermineBestMove() to proceed the current simulation.
*/

short EvaluatePosition(RoundState state, short depth)
{
  short rate;
  short code = FindWinner(state);
  
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

short DetermineBestMove(RoundState state, short *MoveRating)
{
  short BestX;
  short i, max = -1;

  if(state.Moves==0) return (MaxX+1)/2;
  
  BestX = EvaluateBestMove(state, MoveRating, 0);

  //for(i=0;i<=MaxX;i++)
  //{
  //  printf("%d\t",VictoryProbability[i]);
  //}
  //printf("\n");
  
  if(*MoveRating == NeutralPosition)
  {
    //printf("\n\nGET\n\n");
    // Find the maximum probability
    for(i=0;i<=MaxX;i++)
    {
      if(VictoryProbability[i] > max&&state.NextMove[i][0] != -1)
      {
        max = VictoryProbability[i];
        BestX = i;
      }
    }
  }
  
  // Clean up VictoryProbability for the next round.
  for(i=0;i<=MaxX;i++)
  {
    VictoryProbability[i] = 0;
  }
  
  return BestX;
}

// A beautiful way to be able to use printf()

int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

void setup()
{
  Serial.begin(9600);
  printf_begin();
  //randomSeed(analogRead(0));
}

int GetDigit()
{
  String inString;
  int i;
  //while(1)
  //{
    while (1)//(Serial.available() > 0)
    {
      int inChar = Serial.read();
      if (isDigit(inChar)) 
      {
        inString += (char)inChar;
        i=inString.toInt();
        return i;
      }
    }
  //}
}

void loop()
{
  GameMain();

  printf("\nPress any digit...");
  GetDigit();
}
