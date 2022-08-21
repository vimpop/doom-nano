#include "input.h"

static const uint8_t row[] = {PC10, PC11};          // Row 4 & row 5
static const uint8_t col[] = {PA4, PA15, PA9, PA8}; // col13,col21,col17,col20
static uint32_t KEY = 0;
void input_setup()
{
  for (int x = 0; x < sizeof(col); x++)
  {
    pinMode(col[x], INPUT_PULLUP);
  }
  for (int x = 0; x < sizeof(row); x++)
  {
    pinMode(row[x], OUTPUT);
    digitalWrite(row[x], HIGH);
  }
  pinMode(PB7, OUTPUT);
}
void input_matrix_data()
{
  KEY = 0;
  for (int x = 0; x < sizeof(row); x++)
  {
    digitalWrite(row[x], LOW);
    for (int y = 0; y < sizeof(col); y++)
    {
      KEY |= !digitalRead(col[y]) << x * sizeof(row) + y;
      delayMicroseconds(60);
    }
    digitalWrite(row[x], HIGH);
  }
}
bool input_left()
{
  return KEY & 0x4;
};

bool input_right()
{
  return KEY & 0x10;
};

bool input_up()
{
  return KEY & 0x2;
};

bool input_down()
{
  return KEY & 0x8;
};

bool input_fire()
{
  return KEY & 0x20;
};
bool input_exit()
{
  return KEY & 0x1;
}
