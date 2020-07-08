#include "encoder.h"

const int in1 = 49;
const int in2 = 51;
const int in3 = 50;
const int in4 = 48;
int ENB1 = 7;
int ENA2 =6;
int rotateAngle;
int movingDistance;
int revCoefficient = 0; //todo calculate coef, represent how count of encoder revolutions relate to distance

void pinsSetup(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode (ENB1, OUTPUT);
  pinMode (ENA2, OUTPUT);
}

void setRotateAngle(int angle){
  rotateAngle = angle;
}

void setMovingDistance(distance){
  movingDistance = distance;
}

void moveForward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENB1,150);
  analogWrite(ENA2,150);
}

void moveTo(){
  moveForward();
  while (getRevolutions() < movingDistance * revCoefficient)
  {
    continue;
  }
  stop();
}

void rotate(int angle){
    if(angle < 0){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(ENB1,150);
      analogWrite(ENA2,150);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(ENB1,150);
      analogWrite(ENA2,150);
    }
}

void stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ENB1,75);
  analogWrite(ENA2,75);
}

void performRotationToAngle(){
  int delayTime = 4.5 * rotateAngle;
  rotate(rotateAngle);
  delay(delayTime);//todo delete delay and add stop rotation by encoder values
  stop();
}
