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

NewPing sonar(Trig, Echo, MAX_DISTANCE);// переменная для датчика расстояния
Servo servo;// переменная для серво привода
enum { ENC_PIN1 = 28, ENC_PIN2 = 30 };// переменная для Энкодора
enum { FORWARD = 1, BACKWARD = -1 };// переменная для Энкодора
const int in1 = 49; // подключения выводов моторов к ардуине, это те порты куда подключено управление моторами
const int in2 = 51; // подключения выводов моторов к ардуине, это те порты куда подключено управление моторами
const int in3 = 50; // подключения выводов моторов к ардуине, это те порты куда подключено управление моторами
const int in4 = 48; // подключения выводов моторов к ардуине, это те порты куда подключено управление моторами
boolean direct;// переменный для поворота сервопривода НЕ ТРОГАТЬ
boolean hold_flag;// переменный для поворота сервопривода НЕ ТРОГАТЬ
boolean next;// переменный для поворота сервопривода НЕ ТРОГАТЬ
boolean recievedFlag;// переменная для 2 функции офлаг о получении данных
int ENB1 = 7;// переменная для управления скоростью моторов и порт к которому оно подключено
int ENA2 =6;// переменная для управления скоростью моторов и порт к которому оно подключено
int b=0;// лишнии переменные , были какие-то эксперементы 
int c=0; // лишнии переменные , были какие-то эксперементы 
int Ugol=0;// Переменная для угла поворота машинки
int angle = MIN_ANGLE;// перменная для угла поворота серво привода НЕ ТРОГАТЬ
int number=0; // переменная для Выбора функции (действия что будет делать машинка)
unsigned int distance = 100;// перменная для пройденного расстояния НЕ ТРОГАТЬ
int vremia;
int direction = FORWARD;
String a="";

long revolutions = 0;
uint8_t previous_code = 0;


void setup() 
{  
Serial.begin(9600); // Объявляю скорость работы сериал порт
// настройки для Сервы
servo.attach(10);
servo.write(MIN_ANGLE);
//настройки для Ультразвукового датчик
pinMode(Trig, OUTPUT); // выход
pinMode(Echo, INPUT); // вход
// Настройки для мотора
pinMode(in1, OUTPUT); // выход на L298n(управление моторов
pinMode(in2, OUTPUT); // выход на L298n(управление моторов
pinMode(in3, OUTPUT); // выход на L298n(управление моторов
pinMode(in4, OUTPUT); // выход на L298n(управление моторов
pinMode (ENB1, OUTPUT);// выход на управление скоростью моторов
pinMode (ENA2, OUTPUT);// выход на управление скоростью моторов
pinMode(ENC_PIN1, INPUT);// выход на энкодоры
pinMode(ENC_PIN2, INPUT);// выход на энкодоры
} 

void loop(){
  while (Serial.available() == 0){} // не помню что за говно но оно надо
  number=Serial.read()-'0'; // получение номера команды. Номер 1 Стоять на месте, Номер 2 Повернуться, номер 3 ехать прямо
  if(number == 1){ReadyToGetTarget();number=0;} // Команда номер один
  if(number == 2){while (Serial.available() == 0){} // Команда номер 2 поворот на угол. ждем данных из Serial port
                         while (Serial.available() > 0) { //Если данные больше 0 то ссчитываем строку
                         a=Serial.readString();// считывание строки // забиваем строку принятыми данными
                         char *z = new char[10]; //Создание масива для перевода
                         a.toCharArray(z,10);    //Перевод символных данных в тип Char
                         Ugol=atoi(z);           //Перевод Char данных в INT данные
                         recievedFlag = true;    //// поднять флаг что получили данные
                         z="";// отчистка вспомогательного масива
                          delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
                                 }
                         if (recievedFlag) {                      // если данные получены
                              Serial.println(Ugol); // напечатать угол в монитор порта для визуально контроля
                               RotateToTarget(Ugol); // функция поворота на угол. все функции в низу
                               if(Ugol==-90)delay(400);// если угол равен -90 то время выполнения команды 400 мс
                               if(Ugol==90)delay(400);// если угол равен 90 то время выполнения команды 400 мс
                               if(Ugol==-180)delay(700);// если угол равен -180 то время выполнения команды 700 мс
                               if(Ugol==180)delay(700);// если угол равен 180 то время выполнения команды 700 мс
                               ReadyToGetTarget(); // функция для остановки машинки 
                               Ugol = 0; // обнуление переменной угол
                               a="";// очистка масива приема данных
                               recievedFlag = false;}number=0;b=0;} // сброс флаг, высставление переменной обозначающй номер команды в 0
  if(number == 3){// 3 команда ехать прямо
    while(distance>15 || distance==0){ // пока дистанция до препятсвия больше 15 см или равняеться 0 (датчик глючит иногда 0 показывает)
      GoToTarget();// функция ехать прямо
      TurnServo();// функция поворота серво привода на нем стоит датчик
      Encoder();// функция считывания пройденного расстояния  по энкодору
      distance = sonar.ping_cm(); // переменная дистанции до препядствия 
      Serial.println("None");}// это лишнее
      Serial.println("Расстояние до препядствия:");// отправка данных о пройденном расстоянии и расстоянии до цели
      Serial.println(distance);
      Serial.println("Пройденное расстояние:");
      Serial.println(revolutions);
      distance=100;// задание начальных значений переменным о дистанции до цели 
      revolutions=0;// пройденное расстояние
      ReadyToGetTarget();// функция остановки на месте
      number=0;}// обнуление номера действия.
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
