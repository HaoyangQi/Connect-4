
void EstablishConnection()
{
  while(Serial.available()<=0)
  {
  }
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  EstablishConnection();
  
  if (Serial.available() > 0)
  {
    byte recv = Serial.read();
    //if(recv==100)
    {
      Serial.write(recv);
    }
  }
}
