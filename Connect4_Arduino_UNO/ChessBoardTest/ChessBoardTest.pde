static final int WIDTH = 500;
static final int HEIGHT = 500;
final int BLOCKX = WIDTH / 8;
final int BLOCKY = HEIGHT / 8;

boolean ifInit=false;
int curX,curY;
boolean ifMakeAMove=false;
int player=1;

int[][] Scene={{-1,-1,-1,-1,-1,-1,-1},
               {-1,-1,-1,-1,-1,-1,-1},
               {-1,-1,-1,-1,-1,-1,-1},
               {0, 0, 0, 0, 0, 0, 0}};

int[][] NextMove={{0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3}};

PFont font;
int fontHeight = 30;
int bkColor = 102;
int textColor = 255;

// Uses coordinates under window system
boolean CheckNextStep(int x, int y)
{
  int i = 0;
  
  if(y == -1)
    return false;
  
  for(i=0;i<=6;i++)
  {
    if(NextMove[i][0] == -1)
    {
      continue;
    }
    else if(NextMove[i][0] == x && NextMove[i][1] == y)
    {
      return true;
    }
  }
  
  return false;
}

void UpdateText(String s)
{
  if(s==null) return;
  noStroke();
  fill(bkColor);
  rect(0,height-fontHeight,width,fontHeight+5);// erase text
  stroke(0);
  fill(textColor);
  text(s, 0, height-5);
}

void DrawBoard()
{
  for (int i=0;i<8;i++)
  {
    for (int j=0;j<8;j++)
    {
      fill(255, 255, 255);
      rect(i * BLOCKX, j * BLOCKY, (i + 1) * BLOCKX, (j + 1) * BLOCKY);     
    } 
  }
}

void MakeMove(int x, int y)
{
  if(player==1) fill(255, 0, 0);
  else fill(0, 255, 255);
  rect(x, y, BLOCKX, BLOCKY);
  
  int ArrX = x/BLOCKX;
  int ArrY = y/BLOCKY;
  
  // Mark that position
  Scene[ArrY][ArrX] = player;
  
  // Update the NextMove Matrix
  if(ArrY > 0)
  {
    Scene[ArrY-1][ArrX] = 0;
    NextMove[ArrX][0] = ArrX;
    NextMove[ArrX][1] = ArrY-1;
  }
  // If we reach the top, disable this column
  else if(ArrY == 0)
  {
    NextMove[ArrX][0] = -1;
    NextMove[ArrX][1] = -1;
  }
  
  // Reverse the player
  player = (player==1)?2:1;
  
  UpdateText("("+ArrX+","+ArrY+")");
}

void setup() {
  size(435, 279); //y:525-60*4-6
  //noLoop(); 
  font = createFont("Courier New",32,true);
  textFont(font);
  textSize(fontHeight);
}
 
void draw()
{
  if(!ifInit)
  {
    DrawBoard();
    ifInit=true;
  }
  
  if(ifMakeAMove==true)
  {
    MakeMove(curX,curY);
    ifMakeAMove=false;
  }
  
  // If the current turn is for Arduino...
  // Wait here for Arduino to respond then receive/process the data
  // and update the board again.
  // We trust the coordinate that Arduino gives us is legal :)
  // Exception: If the coordinate is invalid, do nothing.
  if(player==2)
  {
    UpdateText("Arduino is thinking...");
  }
}

void mouseClicked()
{
  if(player==1)
  {
    // Mapping coordinates to window
    curX = (mouseX/BLOCKX)*BLOCKX;
    curY = (mouseY/BLOCKY)*BLOCKY;
    
    // Mapping coordinates to array
    if(CheckNextStep(curX/BLOCKX,curY/BLOCKY))
    {
      ifMakeAMove=true;
      // It is time to send this to Arduino to synchornize
    }
    else
      UpdateText("Invalid move, try again.");
  }
}