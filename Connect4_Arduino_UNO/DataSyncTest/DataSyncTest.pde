import processing.serial.Serial;

Serial myPort=null;
boolean FirstHandShake=false;
String name = "COM3";
int tmp=0;

PFont font;
int fontHeight = 30;
int bkColor = 102;
int textColor = 255;

void UpdateText(String s)
{
  if(s==null) return;
  noStroke();
  fill(bkColor);
  rect(0,0,width,fontHeight+5);// erase text
  stroke(0);
  fill(textColor);
  text(s, 0, fontHeight);
}

void setup()
{
  size(800,600);
  font = createFont("Courier New",32,true);
  textFont(font);
  textSize(fontHeight);
  //textAlign(LEFT);
  smooth();
  background(bkColor);
  
  try
  {
    myPort = new Serial(this, name, 9600);
  }
  catch(Exception e)
  {
    exit();
  }
  
  myPort.bufferUntil('\n');
}

void draw()
{
  //serialEventRun();
  //serialEvent_Receive(myPort);
}

// Interesting thing is: serialEvent will not be actived when data comes in

void serialEvent_Receive(Serial p)
{
  
  //String myString = p.readStringUntil('\n');
  int recv=p.read();
  
  //if(myString!=null)
  {
    UpdateText("Message: "+recv+" ("+tmp+")"); tmp++;
  }
  
}

// In order to avoid interlocking problem, 
// we use a positive connecting method:
//
// Arduino will play the passive end, wating 
// to be actived.
// Processing end will send acivation signal 
// (or data) when it wants Arduino to work.
void mouseClicked()
{
  byte trans = 100;
  myPort.write(trans);
  serialEvent_Receive(myPort);
}