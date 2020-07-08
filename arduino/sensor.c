#include <NewPing.h>

#define Trig 15
#define Echo 14
#define MAX_DISTANCE 200

void sensorSetup(){
    NewPing sonar(Trig, Echo, MAX_DISTANCE);
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
}

int getDistance(){
    return sonar.ping_cm();
}
