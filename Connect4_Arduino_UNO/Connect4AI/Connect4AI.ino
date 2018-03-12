#include "connect4.h"

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

const short Direction[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

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

#define WinPosition     1000
#define LosePosition    -1000
#define NeutralPosition 0

// It is ok for 6, but it will take nearly 2 minutes.
// It is bad for user...
#define MaxDepth 5

short VictoryProbability[MaxX+1] = {0};

short EvaluatePosition(RoundState, short);

short CriticalCnt=0,CriticalPos=-1;

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
  //if(CriticalCnt==MaxX && depth==0)
  if(CriticalCnt>0 && depth==0)
  {
    *MoveRating = WinPosition; // Hack the move rating
    BestMoveX = CriticalPos;
    CriticalCnt=0;
    CriticalPos=-1;
  }
  return BestMoveX;
}

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

short DetermineBestMove(RoundState state, short *MoveRating)
{
  short BestX;
  short i, max = -1;

  BestX = (MaxX+1)/2;
  if(state.Moves==1) return (state.NextMove[BestX][1] == MaxY)?BestX:(BestX-1);
  
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

void setup()
{
  Serial.begin(9600);
}

RoundState game;

void loop()
{
  int recv,x,y,rating;
  
  while(Serial.available()<=0);

  recv = Serial.read();

  // Initialize signal
  if(recv==100)
  {
    GameInit(&game, PLAYER_A);
    // send the signal back to finish hand-shake process
    Serial.write(recv);
    return;
  }

  // Game end OR user's turn
  // We do nothing this round
  else if(recv==200)
  {
    return;
  }

  // A Coordinate
  else
  {
    // Update first
    y = CalculateCoordinateY(game, recv);
    MakeMove(&game, recv, y);
    //delay(2000);
    // AI part
    x = DetermineBestMove(game, &rating);
    y = CalculateCoordinateY(game, x);
    /*
    if(y==-1)
    {
      x++;
      y = CalculateCoordinateY(game, x);
    }\
    */
    // A hand-shake for data synchornization
    //while(Serial.available()<=0);
    // Clear buffer
    while(Serial.available()>0) Serial.read();
    Serial.write(x);
    //Serial.write((byte)-1);
    MakeMove(&game, x, y);
    return;
  }
}
