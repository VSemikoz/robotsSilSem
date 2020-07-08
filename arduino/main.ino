#include <GyverHacks.h>
#include <string.h>
#include <Servo.h>
#include <NewPing.h>

#include "engine.h"
#include "servo.h"
#include "sensor.h"
#include "encoder.h"
#include "toolsFunc.h"



void setup(){
    pinsSetup();
    servoSetup();
    sensorSetup();
    encoderSetup();

    Thread rotateThread = Thread();
    rotateThread.onRun(performRotationToAngle)

    Thread movingThread = Thread();
    movingThread.onRun(moveTo)

    Thread servoThread = Thread();
    servoThread.onRun(TurnServo)
    servoThread.run()
}

void loop(){
    while (Serial.available() == 0){}
    commandString = Serial.readLine();
    commandArray = split(commandString);
    commandType = commandArray[0];
    commandParams = removeFirst(commandArray);

   """AVAILABLE_COMMANDS = {quit: 0,
                      rotate: 1,
                      move_forward: 2}"""
    if (commandType == 0){
        //perform quit/power off...
        return
    }
    if (commandType == 1){
        //perform rotation on angle commandParams[0]
        int angle = commandParams[0]
        setRotateAngle(angle);
        rotateThread.run();
    }
    if (commandType == 2){
        //perform moving forfard to distance commandParams[0]
        int distance = commandParams[0]
        setMovingDistance(distance);
        movingThread.run()
    }
}