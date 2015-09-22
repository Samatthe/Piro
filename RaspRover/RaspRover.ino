//true: right-fw  left-rev
int readmax = 255;
int readmin = 205;
int v = 0;
int wx = 50;
int wy = 50;
int x = 0;
boolean pressed = false;
int key = 0;
int sright = 0;
boolean dright = true;
int sleft = 0;
boolean dleft = false;
int ycomp = 0;

///////////// Senor Pins ///////////////////
int const S1PWR = 0;
int const S1GND = 1;
int const S1TX = 2;
int const S1RX = 3;
int const S1AN = 4;

int const S2PWR = 5;
int const S2GND = 6;
int const S2TX = 7;
int const S2RX = 8;
int const S2AN = 9;
////////////////////////////////////////////

/////////// Serov Pins ////////////////////
int const SER1PWR = 10;
int const SER1GND = 11;
int const SER1PWM = 12;

int const SER2PWR = 13;
int const SER2GND = 14;
int const SER2PWM = 15;
//////////////////////////////////////////

/////////// Mode Variables ////////////////
int Mode = 0;
///////////////////////////////////////////


void setup()
{
  Serial.begin(57600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(SER1PWM, OUTPUT);
  pinMode(SER2PWM, OUTPUT);
  pinMode(S1TX, INPUT);
  pinMode(S2TX, INPUT);
  pinMode(S1RX, OUTPUT);
  pinMode(S2RX, OUTPUT);
  pinMode(S1AN, INPUT);
  pinMode(S2AN, INPUT);
}

void loop()
{
  if (Serial.available())
  {
    char in = Serial.read();
    switch (in)
    {
      case '0'...'9':
        v = (v * 10) + (in - '0');
        break;

      case 'X':
      case 'x':
        {
          if (Mode == 0)
          {
            wx = v;
            if (wx < 100)
            {
              wx = constrain(wx, 0, 50);
              Serial.print(wx);
              wx = -(wx - 50);
            }
            else if (wx > 100)
            {
              wx = constrain(wx, 205, 255);
              Serial.print(wx);
              wx = -(wx - 255) + 50;
            }
            Serial.print("                   ");
            Serial.print(v);
            Serial.print("  ");
            v = 0;
          }
        }
        break;

      case 'Y':
      case 'y':
        {
          if (Mode == 0)
          {
            wy = v;
            if (wy < 100)
            {
              wy = constrain(wy, 0, 50);
              wy = -(wy - 50);
            }
            else if (wy > 100)
            {
              wy = constrain(wy, 205, 255);
              wy = -(wy - 255) + 50;
            }
            v = 0;
            break;

          case 'B':
            pressed = true;
            if (v == 17)
              key = 1;
            else if (v == 33)
              key = 2;
            else if (v == 49)
              key = 3;

            v = 0;
          }
        }
        break;
    }
  }

  if (key == 1 && pressed && !lpressed) // Increment the Mode if up pressed
    ++Mode;
  if (key == 2 && pressed && !lpressed) // Increment the Mode if up pressed
    --Mode;
  if (Mode >= Modes)                   // Check Mode is in range
    Mode = 0;
  if (Mode < 0)
    Mode = Modes - 1;

  switch (Mode)
  {
    case 0:
      steer();
      break;
    case 1:
      manDist();
      break;
  }

  sright = constrain(sright, 0, 255);
  sleft = constrain(sleft, 0, 255);
  drive(dright, sright, dleft, sleft);

  lpressed = pressed;
  pressed = false;
}

void drive(boolean dir1, int sp1, boolean dir2, int sp2)
{
  digitalWrite(2, dir1);
  digitalWrite(7, (!dir1));
  analogWrite(3, sp1);
  analogWrite(6, sp1);
  digitalWrite(4, (!dir2));
  digitalWrite(8, dir2);
  analogWrite(5, sp2);
  analogWrite(11, sp2);
}

void steer()
{
  if (wy > 51)
  {
    ycomp = map(wy, 53, 100, 0, -255);
    Serial.print(ycomp);
    Serial.print("   ");
    //Serial.println(wy);
  }
  else if (wy < 49)
  {
    ycomp = -(wy - 49);
    ycomp = map(ycomp, 2, 49, 0, 255);
    Serial.print(ycomp);
    Serial.print("   ");
    //Serial.println(wy);
  }
  else
  {
    ycomp = 0;
    //Serial.print(wy);
  }

  if (wx > 51)
  {
    x = map(wx, 53, 100, 0, 255);

    sright = abs(x - ycomp); //handle right speed with turn compensation
    dright = !(ycomp > x);  // we need to change direction if speed is negative

    sleft = abs(x + ycomp); // handle left speed with turn compensation
    dleft = !((x + ycomp) >= 0); // we need to change direction if speed is negative

    Serial.print(x);       // print to screen for debugging
    Serial.print("   ");
    Serial.println(wx);
  }
  else if (wx < 49)
  {
    x = -(wx - 49);
    x = map(x, 2, 49, 0, 255);

    sright = abs(x - ycomp); //handle right speed with turn compensation
    dright = (ycomp > x);  // we need to change direction if speed is negative

    sleft = abs(x + ycomp);
    dleft = ((x + ycomp) >= 0); // we need to change direction if speed is negative

    Serial.print(x);
    Serial.print("   ");
    Serial.println(wx);
  }
  else
  {
    /*sright = abs(-ycomp); //handle right speed with turn compensation
    if(ycomp>0)     // we need to change direction if speed is negative
    dright = false;
    else
    dright = true;

    sleft = abs(ycomp);  // handle left speed with turn compensation
    if((ycomp)>=0)     // we need to change direction if speed is negative
    dleft = false;
    else
    dleft = true;
    Serial.println(wx);*/
  }
}

void manDist()
{
  // sweep left to right with both sensors finding the min distance
  // limit servo range to stop cross feed
  //
}

void autoDist()
{
  // sweep left to right 90deg with both sensors counter of eachother
  //
  //
}

void manBubble()
{

}
