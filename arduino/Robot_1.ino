#define Trig 15
#define Echo 14
#define MAX_ANGLE 140   // максимальный угол поворота сервы 
#define MIN_ANGLE 50      // минимальный угол поворота сервы
#define STEP_DELAY 15 
#define MAX_DISTANCE 200

#include <GyverHacks.h>
GTimer stepTimer(STEP_DELAY);
#include <string.h> 
#include <Servo.h>
#include <NewPing.h>

NewPing sonar(Trig, Echo, MAX_DISTANCE);
Servo servo;
enum { ENC_PIN1 = 28, ENC_PIN2 = 30 };
enum { FORWARD = 1, BACKWARD = -1 };
const int in1 = 49; 
const int in2 = 51; 
const int in3 = 50; 
const int in4 = 48; 
boolean direct;
boolean hold_flag;
boolean next;
boolean recievedFlag;
int ENB1 = 7;
int ENA2 =6;
int b=0;
int c=0; 
int Ugol=0;
int angle = MIN_ANGLE;
int number=0;
unsigned int distance = 100;
int vremia;
int direction = FORWARD;
String a="";

long revolutions = 0;
uint8_t previous_code = 0;


void setup() 
{  
Serial.begin(9600);
// настройки для Сервы
servo.attach(10);
servo.write(MIN_ANGLE);
//настройки для Ультразвукового датчик
pinMode(Trig, OUTPUT); // выход
pinMode(Echo, INPUT); // вход
// Настройки для мотора
pinMode(in1, OUTPUT); // выход на L298n
pinMode(in2, OUTPUT); // выход на L298n
pinMode(in3, OUTPUT); // выход на L298n
pinMode(in4, OUTPUT); // выход на L298n
pinMode (ENB1, OUTPUT);
pinMode (ENA2, OUTPUT);
pinMode(ENC_PIN1, INPUT);
pinMode(ENC_PIN2, INPUT);
} 

void loop(){
  while (Serial.available() == 0){}
  number=Serial.read()-'0';
  if(number == 1){ReadyToGetTarget();number=0;}
  if(number == 2){while (Serial.available() == 0){}
                         while (Serial.available() > 0) { 
                         a=Serial.readString();
                         char *z = new char[10];
                         a.toCharArray(z,10);    
                         Ugol=atoi(z);        // забиваем строку принятыми данными
                         recievedFlag = true;
                         z="";// поднять флаг что получили данные
                          delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
                                 }
                         if (recievedFlag) {                      // если данные получены
                              Serial.println(Ugol);
                               RotateToTarget(Ugol);
                               if(Ugol==-90)delay(400);
                               if(Ugol==90)delay(400);
                               if(Ugol==-180)delay(700);
                               if(Ugol==180)delay(700);
                               ReadyToGetTarget();
                               Ugol = 0;
                               a="";// очистить
                               recievedFlag = false;}number=0;b=0;}
  if(number == 3){
    while(distance>15 || distance==0){
      GoToTarget();
      TurnServo();
      Encoder();
      distance = sonar.ping_cm();
      Serial.println("None");}
      Serial.println("Расстояние до препядствия:");
      Serial.println(distance);
      Serial.println("Пройденное расстояние:");
      Serial.println(revolutions);
      distance=100;
      revolutions=0;
      ReadyToGetTarget();
      number=0;}
}
void TurnServo(){
  if (direct) {             // движемся в прямом направлении
    if (angle < MAX_ANGLE)
      turn_to(MAX_ANGLE);   // плавный поворот
    else {
      direct = false;       // смена направления
      delay(50);            // задержка в крайнем положении
    }
  }
  else {                    // движемся в обратном направлении
    if (angle > MIN_ANGLE)
      turn_to(MIN_ANGLE);   // плавный поворот
    else {
      direct = true;        // смена направления
      delay(50);            // задержка в крайнем положении
    }
  }
}
void turn_to(byte to_angle) {// Функция для плавного поворота сервы
  if (stepTimer.isReady()) {
    if (angle < to_angle) angle++;
    else if (angle > to_angle) angle--;
    else hold_flag = true;
    servo.write(angle);
    next = true;
  }
}

void ReadyToGetTarget(){          
 
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
analogWrite(ENB1,75);
analogWrite(ENA2,75);
}

int RotateToTarget(int Ugol)
{ 
  if(Ugol==-180 || Ugol==-90 ){  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ENB1,150);
  analogWrite(ENA2,150);
  }
  if(Ugol==180 || Ugol==90){  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENB1,150);
  analogWrite(ENA2,150);
  }
}

void GoToTarget(){
  digitalWrite(in1,LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENB1,150);
  analogWrite(ENA2,150);
}
void Encoder(){
  uint8_t code = readEncoder(ENC_PIN1, ENC_PIN2);
  if (code == 0)
  {
    if (previous_code == 3)
      turned(FORWARD);
    else if (previous_code == 1)
      turned(BACKWARD);}
  previous_code = code;
}
void turned(int new_direction)
{
  if (new_direction != direction)
  {
    revolutions = 0;
  }
  else
    ++revolutions;

  direction = new_direction;
}

/* Объеденил чтение кода Грея с энкодера с его декодированием */
uint8_t readEncoder(uint8_t pin1, uint8_t pin2)
{
  uint8_t gray_code = digitalRead(pin1) | (digitalRead(pin2) << 1), result = 0;

  for (result = 0; gray_code; gray_code >>= 1)
    result ^= gray_code;

  return result;
}
