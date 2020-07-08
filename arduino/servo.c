#include <GyverHacks.h>
#include <Servo.h>

#define MAX_TURN_ANGLE 140
#define MIN__TURN_ANGLE 50
#define STEP_DELAY 15

GTimer stepTimer(STEP_DELAY);
Servo servo;
boolean direct;
int angle = MIN_ANGLE;

int getCurrentAngle(){
    return angle;
}

void servoSetup(){
    servo.attach(10);
    servo.write(MIN_ANGLE);
}

void TurnServo(){
  if (direct) {
    if (angle < MAX_ANGLE)
      turn_to(MAX_ANGLE);
    else {
      direct = false;
    //   delay(50);
    }
  }
  else {
    if (angle > MIN_ANGLE)
      turn_to(MIN_ANGLE);
    else {
      direct = true;
    //   delay(50);
    }
  }
}

void turn_to(byte to_angle) {
  if (stepTimer.isReady()) {
    if (angle < to_angle) angle++;
    else if (angle > to_angle) angle--;
    servo.write(angle);
  }
}