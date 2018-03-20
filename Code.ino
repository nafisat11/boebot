#include <Servo.h>
#define FIND_BOARD 0
#define FOLLOW_BOARD 1
#define FIND_CUP 2
#define MANEUVER_CUP 3
#define FIND_DEST 4
#define TERMINATE 5

//Global variables
Servo servoLeft;
Servo servoRight;
int State;

void setup() {
  pinMode(9, OUTPUT); //Assigning pin to IR LED
  pinMode(8, INPUT); //Assigning pin to Receiver

  pinMode(5, OUTPUT);
  pinMode(12, INPUT);

  pinMode(4, OUTPUT);
  pinMode(6, INPUT);
  
  servoLeft.attach(11); 
  servoRight.attach(10);

  delay(1000);
  
  State = FIND_BOARD;
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1150);
  delay(800);
}

void loop() { 
  int irFront = irDetect (4, 6, 38500);
  int irLeft = irDetect (9, 8, 38000);
  int irRight = irDetect (5, 12, 38000);

  int FrontObstruction, LeftObstruction, RightObstruction;

  switch(State){
    case FIND_BOARD:
      FrontObstruction = irFront;
      break;

    case FOLLOW_BOARD:
      LeftObstruction = irLeft;
      break;

    case FIND_CUP:
      FrontObstruction = irFront; 
      LeftObstruction = irLeft;
      RightObstruction = irRight;
      break;

    case MANEUVER_CUP:
      FrontObstruction = irFront; 
      LeftObstruction = irLeft;
      RightObstruction = irRight;
      break;

    case FIND_DEST:
      FrontObstruction = irFront; 
      LeftObstruction = irLeft;
      RightObstruction = irRight;
      break;
    
    case TERMINATE:
    break;
  }

  switch(State){
    case FIND_BOARD:
      if(FrontObstruction == 0){
        StopPivotTurnRight();
        State = FOLLOW_BOARD;
        servoLeft.writeMicroseconds(1600);
        servoRight.writeMicroseconds(1150);
        delay(900);
      }
      break;
      
    case FOLLOW_BOARD:
      if(LeftObstruction == 1){
        StopPivotTurnLeft();
        servoLeft.writeMicroseconds(1600);
        servoRight.writeMicroseconds(1150);
        delay(700);
        State = FIND_CUP;
        StopPivotTurnLeft(); 
      }
      break;

    case FIND_CUP:
      if(LeftObstruction == 0){
        servoLeft.writeMicroseconds(1650);
        servoRight.writeMicroseconds(1200);
        delay(3600);
        StopPivotTurnRight();
        State = MANEUVER_CUP;
        ForwardSlowly();
      }
      break;

    case MANEUVER_CUP:
      if(FrontObstruction == 0 && LeftObstruction == 1){
        CupDetection();
        
        servoLeft.writeMicroseconds(1650);
        servoRight.writeMicroseconds(1200);
        delay(1000);
        State = FIND_DEST;
      }
      break;

    case FIND_DEST:
      if(FrontObstruction == 1 && LeftObstruction == 1 && RightObstruction == 1){
        StopMoving();
        State = TERMINATE;
      }
      break;

    case TERMINATE:
      servoLeft.detach();
      servoRight.detach();
      break;
  }
}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);
  delay(1);
  int ir = digitalRead(irReceiverPin);
  delay(1);
  return ir; //ir = 0, detect. ir = 1, not detected
}

void StopMoving()
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(500);
}

void ForwardSlowly()
{
  servoLeft.writeMicroseconds(1525);
  servoRight.writeMicroseconds(1470);
  delay(100);
}

void StopPivotTurnRight()
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(300);

  servoLeft.writeMicroseconds(1650);
  servoRight.writeMicroseconds(1500);
  delay(1070);
}

void StopPivotTurnLeft()
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(300);

  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1400);
  delay(1130);
}

void CupDetection()
{
  while(1){
    int irFront = irDetect (4, 6, 38500);
    delay(5);
    int irLeft = irDetect (9, 8, 38000);
    delay(5);
    int irRight = irDetect (5, 12, 38000);
    delay(5);
    
    if(irFront == 1 && (irLeft == 0 || irRight == 0)){
      break;
    }
    servoLeft.writeMicroseconds(1515);
    servoRight.writeMicroseconds(1515);
    delay(230);
  }
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1500);
    delay(300);

    
   while(1){
    int irLeft = irDetect (9, 8, 38000);
    delay(10);
    if(irLeft == 1)
    {
      break;
    }
    servoLeft.writeMicroseconds(1560);
    servoRight.writeMicroseconds(1150);
    delay(4000);

   }

    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1500);
    delay(300);
   
    servoLeft.writeMicroseconds(1650);
    servoRight.writeMicroseconds(1150);
    delay(1000);
}

