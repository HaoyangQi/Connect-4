import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.Serial; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Connect4UI extends PApplet {



Serial myPort=null;
boolean FirstHandShake=false;
String name = "COM3";

static final int WIDTH = 500;
static final int HEIGHT = 500;
final int BLOCKX = WIDTH / 8;
final int BLOCKY = HEIGHT / 8;

boolean ifInit=false;
int curX,curY;
boolean ifMakeAMove=false;
int player=1;
int Moves=0;
boolean IsGameOver = false;

int[][] Scene={{-1,-1,-1,-1,-1,-1,-1},
               {-1,-1,-1,-1,-1,-1,-1},
               {-1,-1,-1,-1,-1,-1,-1},
               {-1,-1,-1,-1,-1,-1,-1},
               {0, 0, 0, 0, 0, 0, 0}};

int[][] NextMove={{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4}};

final short[][] Direction = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

int WinnerStrandDirection=-1;
int WinnerStrandPosX=-1;
int WinnerStrandPosY=-1;

PFont font;
int fontHeight = 30;
int bkColor = 102;
int textColor = 255;

// Uses coordinates under window system
public boolean CheckNextStep(int x, int y)
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

public void UpdateText(String s)
{
  if(s==null) return;
  noStroke();
  fill(bkColor);
  rect(0,height-fontHeight,width,fontHeight+5);// erase text
  stroke(0);
  fill(textColor);
  text(s, 0, height-5);
}

public void DrawBoard()
{
  for (int i=0;i<8;i++)
  {
    for (int j=0;j<8;j++)
    {
      fill(255, 255, 255);
      rect(i * BLOCKX, j * BLOCKY, (i + 1) * BLOCKX, (j + 1) * BLOCKY);     
    } 
  }
  
  for(int i=0;i<=4;i++)
  {
    for(int j=0;j<=6;j++)
    {
      Scene[i][j] = -1;
    }
  }
  
  for(int j=0;j<=6;j++)
  {
    Scene[4][j] = 0;
  }
  
  for(int i=0;i<=6;i++)
  {
    NextMove[i][0] = i;
    NextMove[i][1] = 4;
  }
  
  player = 1;
  
  // Reset the global counter
  Moves = 0;
}

// Array Coordinate System
public int CalculateCoordinateY(int x)
{
  int i;
  
  for(i=0;i<=6;i++)
  {
    if(NextMove[i][0] == x)
    {
      return NextMove[i][1];
    }
  }
  
  return -1;
}

public void MakeMove(int x, int y)
{
  if(player==1) fill(255, 0, 0);
  else fill(0, 0, 255);
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
  
  Moves++;
}

public int FindWinner()
{
  short x,y,m,n;
  short NextX,NextY;
  int id;
  boolean pass;
  
  if(Moves < 7)
  {
    return -1;
  }
  
  for(x=0;x<=6;x++)
  {
    for(y=4;y>=0;y--)
    {
      id = Scene[y][x];
      
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
          
          if((NextX<0 || NextX>6) || (NextY<0 || NextY>4))
          {
            //This strand is a dead strand, so it is not 
            //necessary to step forward along this direction.
            pass = false;
            break;
          }
          
          if(Scene[NextY][NextX] != id)
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
          WinnerStrandDirection = m;
          WinnerStrandPosX = x;
          WinnerStrandPosY = y;
          return id;
        }
      }
    }
  }
  
  return -1;
}

public void setup()
{
   //y:525-60*4-6 // 279+62
  //noLoop(); 
  font = createFont("Courier New",32,true);
  textFont(font);
  textSize(fontHeight);
  
  // Scanning and waiting for the connection is not 
  // necessary and is able to easily cause bugs.
  // So if there is no device ready at the right port, 
  // we terminate the program.
  try
  {
    myPort = new Serial(this, name, 9600);
  }
  catch(Exception e)
  {
    exit();
  }
}

// For every frame, there will be two stages: 
// user makes a move then Arduino does another.
public void draw()
{
  int arduino_moveX=-1, arduino_moveY;
  int code = FindWinner();
  int wsX,wsY;
  
  // Game is over
  if(Moves>=35 || code!=-1 || IsGameOver)
  {
    //told Arduino to wait.
    myPort.write(200);
    
    if(code!=-1)
    {
      // Redraw winner strand in yellow color
      
      wsX = WinnerStrandPosX;
      wsY = WinnerStrandPosY;
      fill(255,255,0);
      for(int i=0;i<4;i++)
      {
        rect(wsX*BLOCKX, wsY*BLOCKY, BLOCKX, BLOCKY);
        wsX+=Direction[WinnerStrandDirection][0];
        wsY+=Direction[WinnerStrandDirection][1];
      }
      
      UpdateText("You "+(player==2?"win.":"lose."));
    }
    else
    {
      // Tie
      UpdateText("Tie.");
    }
    
    // Reset for the next time the mouse clicked
    IsGameOver = true;
    return;
  }
  
  if(!ifInit)
  {
    DrawBoard();
    ifInit=true;
  }
  
  if(!FirstHandShake)
  {
    UpdateText("Press L key to start.");
    return;
  }
  
  if(player==1)
  {
    //UpdateText("Your move.");
    if(ifMakeAMove==true)
    {
      MakeMove(curX,curY);
      ifMakeAMove=false;
      myPort.write(curX/BLOCKX);
      
    }
    return;
  }
  
  // If the current turn is for Arduino...
  // Wait here for Arduino to respond then receive/process the data
  // and update the board again.
  // The way to wait for response: never use while loop to wait for data.
  // Simply use if statement. Otherwiase it will create a interlocking issue.
  // We trust the coordinate that Arduino gives us is legal :)
  // Exception: If the coordinate is invalid, do nothing.
  if(player==2)
  {
    /*
    UpdateText("Arduino is thinking...");
    while (myPort.available() <= 0)
    {
      // Do not write anything here!
    }
    */
    if(myPort.available() <= 0)
    {
      UpdateText("Arduino is thinking...");
    }
    else
    {
      //myPort.clear();
      //myPort.write(150);
      //while(myPort.available() <= 0);
      arduino_moveX = myPort.read();
      //UpdateText("It makes a move at ("+arduino_moveX+")");
      //delay(5000);
      arduino_moveY = CalculateCoordinateY(arduino_moveX);
      // Just in case...
      if(arduino_moveY!=-1)
      {
        MakeMove(arduino_moveX*BLOCKX,arduino_moveY*BLOCKY);
        //UpdateText("It makes a move at ("+arduino_moveX+","+arduino_moveY+")");
      }
      
      // If error occurs, we will stuck here (by not updating 'player').
      //UpdateText("Error "+arduino_moveX);
      // We output the result in any case for debugging.
      UpdateText("It makes a move at ("+arduino_moveX+","+(4-arduino_moveY)+")");
      //player=1;
    }
    return;
  }
}

public void mouseClicked()
{
  if(!FirstHandShake)
  {
    myPort.write(100);
    while(myPort.available()<=0);
    myPort.read();
    FirstHandShake=true;
    return;
  }
  if(IsGameOver==true)
  {
    /*
    IsGameOver = false;
    // Require Initialization
    FirstHandShake = false;
    ifInit = false;
    */
    exit();
  }
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
      // We will do this right after we finish processing 
      // user's move, because we will immediately jump into 
      // waiting loop for data transmission.
    }
    else
      UpdateText("Invalid move, try again.");
  }
}
  public void settings() {  size(435, 341); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Connect4UI" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
