#define ENC_PIN1 28
#define ENC_PIN2 30
#define FORWARD 28
#define BACKWARD 30

uint8_t previous_code;
int direction;
int revolutions;

void encoderSetup(){
    uint8_t previous_code = 0;
    int direction = FORWARD;
}

int getRevolutions(){
    return revolutions;
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

uint8_t readEncoder(uint8_t pin1, uint8_t pin2)
{
  uint8_t gray_code = digitalRead(pin1) | (digitalRead(pin2) << 1), result = 0;

  for (result = 0; gray_code; gray_code >>= 1)
    result ^= gray_code;

  return result;
}
