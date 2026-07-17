#include <Servo.h>

Servo servoB1;
Servo servoB2;
Servo servoB3;
Servo servoB4;

Servo servoH1;
Servo servoH2;
Servo servoH3;
Servo servoH4;
             
Servo servoKN1;
Servo servoKN2;
Servo servoKN3;
Servo servoKN4;

const int servoB1pin = 2; //front right (servo 1)
const int servoB2pin = 3; //back right (servo 2)
const int servoB3pin = 4; //front left (servo 4)
const int servoB4pin = 5; //back left (servo 3)

const int servoH1Pin = 6; // back right (servo 8)
const int servoH2Pin = 7; // Front Right (servo 5)
const int servoH3Pin = 8; // back Left(servo 7)
const int servoH4Pin = 9; // front left (servo 6)

const int servoKN1Pin = 10; // back right (servo 12)
const int servoKN2Pin = 11; // Front Right (servo 9)
const int servoKN3Pin = 12; // back Left (servo 11)
const int servoKN4Pin = 13; // front left (servo 10)

/**********************sit robot******************/
void Sit()
{
    for (int i = 90; i >= 0; i--)
    {
        // Left pair
        servoH2.write(i);
        servoH3.write(i);
        servoKN2.write(i);
        servoKN3.write(i);

        // Right pair
        servoH1.write(180 - i);
        servoH4.write(180 - i);
        servoKN1.write(180 - i);
        servoKN4.write(180 - i);

        delay(50);      // Same speed as Stand
    }

    delay(50);
}

/******************stand robot********************/
void Stand()
{
    for (int i = 0; i <= 90; i++)
    {
        // Left pair
        servoH2.write(i);
        servoH3.write(i);
        servoKN2.write(i);
        servoKN3.write(i);

        // Right pair
        servoH1.write(180 - i);
        servoH4.write(180 - i);
        servoKN1.write(180 - i);
        servoKN4.write(180 - i);

        delay(50);      // Adjust speed here
    }

    delay(50);
}

/****************FORWARD****************/
void walkForward()
{
  // Repeat walking cycle
  for (int step = 0; step < 20; step++)
  {
    //---------------- STEP 1 ----------------//
    for (int i = 90; i >= 70; i--)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }

    // Lift Legs 1 & 4
    for (int k = 90; k >= 70; k--)
    {
      servoKN1.write(k);
      servoKN3.write(k);
      delay(30);
    }

    // Lower Legs 1 & 4
    for (int k = 70; k <= 90; k++)
    {
      servoKN1.write(k);
      servoKN3.write(k);
      delay(30);
    }

    //---------------- STEP 2 ----------------//
    for (int i = 70; i <= 110; i++)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }

    // Lift Legs 2 & 3
    for (int k = 90; k >= 70; k--)
    {
      servoKN2.write(k);
      servoKN4.write(k);
      delay(30);
    }

    // Lower Legs 2 & 3
    for (int k = 70; k <= 90; k++)
    {
      servoKN2.write(k);
      servoKN4.write(k);
      delay(30);
    }

    //---------------- Return Center ----------------//
    for (int i = 110; i >= 90; i--)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }
  }

}

/****************BACKWARD****************/
void walkBackward()
{
  // Repeat walking cycle
  for (int step = 0; step < 20; step++)
  {
    //---------------- STEP 1 ----------------//
    for (int i = 90; i <= 110; i++)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }

    // Lift Legs 1 & 4
    for (int k = 90; k >= 70; k--)
    {
      servoKN1.write(k);
      servoKN3.write(k);
      delay(30);
    }

    // Lower Legs 1 & 4
    for (int k = 70; k <= 90; k++)
    {
      servoKN1.write(k);
      servoKN3.write(k);
      delay(30);
    }

    //---------------- STEP 2 ----------------//
    for (int i = 110; i >= 70; i--)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }

    // Lift Legs 2 & 3
    for (int k = 90; k >= 70; k--)
    {
      servoKN2.write(k);
      servoKN4.write(k);
      delay(30);
    }

    // Lower Legs 2 & 3
    for (int k = 70; k <= 90; k++)
    {
      servoKN2.write(k);
      servoKN4.write(k);
      delay(30);
    }

    //---------------- Return Center ----------------//
    for (int i = 70; i <= 90; i++)
    {
      servoH1.write(i);
      servoH3.write(i);

      servoH2.write(180 - i);
      servoH4.write(180 - i);

      delay(30);
    }
  }

 
}

/*****************right turn**********************/
void Rightmove(){
    for (int step = 0; step < 5; step++)
  {
    //---------------- STEP 1 ----------------//
    for (int i = 70; i <= 110; i++)
    {
      servoB1.write(i);
      servoB3.write(i);

      servoB2.write(180 - i);
      servoB4.write(180 - i);

      delay(80);
    }
    delay(40);
  }

}

/************************left move***************/
void Leftmove(){
    for (int step = 0; step < 5; step++)
  {
    //---------------- STEP 1 ----------------//
    for (int i = 110; i >= 70; i--)
    {
      servoB1.write(i);
      servoB3.write(i);

      servoB2.write(180 - i);
      servoB4.write(180 - i);

      delay(80);
    }
    delay(40);
  }
}



void setup()
{
  Serial.begin(115200);

  servoH1.attach(servoH1Pin);
  servoH2.attach(servoH2Pin);
  servoH3.attach(servoH3Pin);
  servoH4.attach(servoH4Pin);

  servoKN1.attach(servoKN1Pin);
  servoKN2.attach(servoKN2Pin);
  servoKN3.attach(servoKN3Pin);
  servoKN4.attach(servoKN4Pin);

  servoB1.attach(servoB1pin);
  servoB2.attach(servoB2pin);
  servoB3.attach(servoB3pin);
  servoB4.attach(servoB4pin);

  Serial.println("Quadruped Robot Started");

  Stand();

}

void loop()
{
  Stand();
  delay(30000);   // 30 seconds

  Sit();
  delay(30000);

  Stand();
  delay(30000);

  walkForward();
  delay(30000);

  walkBackward();
  delay(30000);

  Leftmove();
  delay(30000);

  Rightmove();
  delay(30000);
}