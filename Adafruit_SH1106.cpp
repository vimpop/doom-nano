/*********************************************************************
This is a library for our Monochrome OLEDs based on SH1106 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SH1106 to SH1106

SH1106 driver similar to SH1106 so, just change the display() method.

However, SH1106 driver don't provide several functions such as scroll commands.


*********************************************************************/

#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"
#include "constants.h"
#include "i2c_sw.h"

// the memory buffer for the LCD

static uint8_t buffer[SH1106_LCDHEIGHT * SH1106_LCDWIDTH / 8] = {0};

#define sh1106_swap(a, b) \
  {                       \
    int16_t t = a;        \
    a = b;                \
    b = t;                \
  }

// the most basic function, set a single pixel
void Adafruit_SH1106::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
  case 1:
    sh1106_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    sh1106_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  // x is which column
  switch (color)
  {
  case WHITE:
    buffer[x + (y / 8) * SH1106_LCDWIDTH] |= (1 << (y & 7));
    break;
  case BLACK:
    buffer[x + (y / 8) * SH1106_LCDWIDTH] &= ~(1 << (y & 7));
    break;
  case INVERSE:
    buffer[x + (y / 8) * SH1106_LCDWIDTH] ^= (1 << (y & 7));
    break;
  }
}

Adafruit_SH1106::Adafruit_SH1106(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT)
{
  cs = CS;
  rst = RST;
  dc = DC;
  sclk = SCLK;
  sid = SID;
  hwSPI = false;
}

// constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
Adafruit_SH1106::Adafruit_SH1106(int8_t DC, int8_t RST, int8_t CS) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT)
{
  dc = DC;
  rst = RST;
  cs = CS;
  hwSPI = true;
}

// initializer for I2C - we only indicate the reset pin!
Adafruit_SH1106::Adafruit_SH1106(int SDA, int SCL) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT)
{
  sclk = dc = cs = sid = -1;
  rst = 4;
  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_Init();
  SW_I2C_initial();
  SW_I2C_initial();
}

void Adafruit_SH1106::begin(uint8_t vccstate, uint8_t i2caddr, bool reset)
{
  _vccstate = vccstate;
  _i2caddr = i2caddr;
  delay(500);
  i2c_port_initial(SW_I2C1);
#if defined SH1106_128_32
  // Init sequence for 128x32 OLED module
  SH1106_command(SH1106_DISPLAYOFF);         // 0xAE
  SH1106_command(SH1106_SETDISPLAYCLOCKDIV); // 0xD5
  SH1106_command(0x80);                      // the suggested ratio 0x80
  SH1106_command(SH1106_SETMULTIPLEX);       // 0xA8
  SH1106_command(0x1F);
  SH1106_command(SH1106_SETDISPLAYOFFSET);   // 0xD3
  SH1106_command(0x0);                       // no offset
  SH1106_command(SH1106_SETSTARTLINE | 0x0); // line #0
  SH1106_command(SH1106_CHARGEPUMP);         // 0x8D
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x10);
  }
  else
  {
    SH1106_command(0x14);
  }
  SH1106_command(SH1106_MEMORYMODE); // 0x20
  SH1106_command(0x00);              // 0x0 act like ks0108
  SH1106_command(SH1106_SEGREMAP | 0x1);
  SH1106_command(SH1106_COMSCANDEC);
  SH1106_command(SH1106_SETCOMPINS); // 0xDA
  SH1106_command(0x02);
  SH1106_command(SH1106_SETCONTRAST); // 0x81
  SH1106_command(0x8F);
  SH1106_command(SH1106_SETPRECHARGE); // 0xd9
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x22);
  }
  else
  {
    SH1106_command(0xF1);
  }
  SH1106_command(SH1106_SETVCOMDETECT); // 0xDB
  SH1106_command(0x40);
  SH1106_command(SH1106_DISPLAYALLON_RESUME); // 0xA4
  SH1106_command(SH1106_NORMALDISPLAY);       // 0xA6
#endif

#if defined SH1106_128_64
  // Init sequence for 128x64 OLED module
  SH1106_command(SH1106_DISPLAYOFF);         // 0xAE
  SH1106_command(SH1106_SETDISPLAYCLOCKDIV); // 0xD5
  SH1106_command(0x80);                      // the suggested ratio 0x80
  SH1106_command(SH1106_SETMULTIPLEX);       // 0xA8
  SH1106_command(0x3F);
  SH1106_command(SH1106_SETDISPLAYOFFSET); // 0xD3
  SH1106_command(0x00);                    // no offset

  SH1106_command(SH1106_SETSTARTLINE | 0x0); // line #0 0x40
  SH1106_command(SH1106_CHARGEPUMP);         // 0x8D
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x10);
  }
  else
  {
    SH1106_command(0x14);
  }
  SH1106_command(SH1106_MEMORYMODE); // 0x20
  SH1106_command(0x00);              // 0x0 act like ks0108
  SH1106_command(SH1106_SEGREMAP | 0x1);
  SH1106_command(SH1106_COMSCANDEC);
  SH1106_command(SH1106_SETCOMPINS); // 0xDA
  SH1106_command(0x12);
  SH1106_command(SH1106_SETCONTRAST); // 0x81
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x9F);
  }
  else
  {
    SH1106_command(0xCF);
  }
  SH1106_command(SH1106_SETPRECHARGE); // 0xd9
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x22);
  }
  else
  {
    SH1106_command(0xF1);
  }
  SH1106_command(SH1106_SETVCOMDETECT); // 0xDB
  SH1106_command(0x40);
  SH1106_command(SH1106_DISPLAYALLON_RESUME); // 0xA4
  SH1106_command(SH1106_NORMALDISPLAY);       // 0xA6
#endif

#if defined SH1106_96_16
  // Init sequence for 96x16 OLED module
  SH1106_command(SH1106_DISPLAYOFF);         // 0xAE
  SH1106_command(SH1106_SETDISPLAYCLOCKDIV); // 0xD5
  SH1106_command(0x80);                      // the suggested ratio 0x80
  SH1106_command(SH1106_SETMULTIPLEX);       // 0xA8
  SH1106_command(0x0F);
  SH1106_command(SH1106_SETDISPLAYOFFSET);   // 0xD3
  SH1106_command(0x00);                      // no offset
  SH1106_command(SH1106_SETSTARTLINE | 0x0); // line #0
  SH1106_command(SH1106_CHARGEPUMP);         // 0x8D
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x10);
  }
  else
  {
    SH1106_command(0x14);
  }
  SH1106_command(SH1106_MEMORYMODE); // 0x20
  SH1106_command(0x00);              // 0x0 act like ks0108
  SH1106_command(SH1106_SEGREMAP | 0x1);
  SH1106_command(SH1106_COMSCANDEC);
  SH1106_command(SH1106_SETCOMPINS);  // 0xDA
  SH1106_command(0x2);                // ada x12
  SH1106_command(SH1106_SETCONTRAST); // 0x81
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x10);
  }
  else
  {
    SH1106_command(0xAF);
  }
  SH1106_command(SH1106_SETPRECHARGE); // 0xd9
  if (vccstate == SH1106_EXTERNALVCC)
  {
    SH1106_command(0x22);
  }
  else
  {
    SH1106_command(0xF1);
  }
  SH1106_command(SH1106_SETVCOMDETECT); // 0xDB
  SH1106_command(0x40);
  SH1106_command(SH1106_DISPLAYALLON_RESUME); // 0xA4
  SH1106_command(SH1106_NORMALDISPLAY);       // 0xA6
#endif

  SH1106_command(SH1106_DISPLAYON); //--turn on oled panel
}

void Adafruit_SH1106::invertDisplay(uint8_t i)
{
  if (i)
  {
    SH1106_command(SH1106_INVERTDISPLAY);
  }
  else
  {
    SH1106_command(SH1106_NORMALDISPLAY);
  }
}

void Adafruit_SH1106::SH1106_command(uint8_t c)
{
  // I2C
  SW_I2C_WriteControl_8Bit(SW_I2C1, SH1106_I2C_ADDRESS, 0x00, c);
}

void Adafruit_SH1106::SH1106_data(uint8_t c)
{
  SW_I2C_WriteControl_8Bit(SW_I2C1, SH1106_I2C_ADDRESS, 0x40, c);
}

void Adafruit_SH1106::display(void)
{

  SH1106_command(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
  SH1106_command(SH1106_SETHIGHCOLUMN | 0x0); // hi col = 0
  SH1106_command(SH1106_SETSTARTLINE | 0x0);  // line #0
  byte height = 64;
  byte width = 132;
  byte m_row = 0;
  byte m_col = 2;

  height >>= 3;
  width >>= 3;
  // Serial.println(width);

  int p = 0;

  byte i, j, k = 0;
  for (i = 0; i < height; i++)
  {

    // send a bunch of data in one xmission
    SH1106_command(0xB0 + i + m_row);    // set page address
    SH1106_command(m_col & 0xf);         // set lower column address
    SH1106_command(0x10 | (m_col >> 4)); // set higher column address

    for (j = 0; j < 8; j++)
    {
      SW_I2C_WriteControl_8Bit_OnlyRegAddr(SW_I2C1, SH1106_I2C_ADDRESS, 0x40);
      for (k = 0; k < width; k++, p++)
      {
        SW_I2C_Write_Data(SW_I2C1, buffer[p]);
        i2c_check_ack(SW_I2C1);
        TIMER__Wait_us(SW_I2C_WAIT_TIME);
      }
      i2c_stop_condition(SW_I2C1);
    }
  }
}


void Adafruit_SH1106::clearDisplay(void)
{
  memset(buffer, 0, (SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8));
}


void Adafruit_SH1106::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  boolean bSwap = false;
  switch (rotation)
  {
  case 0:
    // 0 degree rotation, do nothing
    break;
  case 1:
    // 90 degree rotation, swap x & y for rotation, then invert x
    bSwap = true;
    sh1106_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    // 180 degree rotation, invert x and y - then shift y around for height.
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    x -= (w - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
    bSwap = true;
    sh1106_swap(x, y);
    y = HEIGHT - y - 1;
    y -= (w - 1);
    break;
  }

  if (bSwap)
  {
    drawFastVLineInternal(x, y, w, color);
  }
  else
  {
    drawFastHLineInternal(x, y, w, color);
  }
}

void Adafruit_SH1106::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Do bounds/limit checks
  if (y < 0 || y >= HEIGHT)
  {
    return;
  }

  // make sure we don't try to draw below 0
  if (x < 0)
  {
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if ((x + w) > WIDTH)
  {
    w = (WIDTH - x);
  }

  // if our width is now negative, punt
  if (w <= 0)
  {
    return;
  }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y / 8) * SH1106_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y & 7);

  switch (color)
  {
  case WHITE:
    while (w--)
    {
      *pBuf++ |= mask;
    };
    break;
  case BLACK:
    mask = ~mask;
    while (w--)
    {
      *pBuf++ &= mask;
    };
    break;
  case INVERSE:
    while (w--)
    {
      *pBuf++ ^= mask;
    };
    break;
  }
}

void Adafruit_SH1106::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  bool bSwap = false;
  switch (rotation)
  {
  case 0:
    break;
  case 1:
    // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
    bSwap = true;
    sh1106_swap(x, y);
    x = WIDTH - x - 1;
    x -= (h - 1);
    break;
  case 2:
    // 180 degree rotation, invert x and y - then shift y around for height.
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    y -= (h - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation, then invert y
    bSwap = true;
    sh1106_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  if (bSwap)
  {
    drawFastHLineInternal(x, y, h, color);
  }
  else
  {
    drawFastVLineInternal(x, y, h, color);
  }
}

void Adafruit_SH1106::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color)
{

  // do nothing if we're off the left or right side of the screen
  if (x < 0 || x >= WIDTH)
  {
    return;
  }

  // make sure we don't try to draw below 0
  if (__y < 0)
  {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;
  }

  // make sure we don't go past the height of the display
  if ((__y + __h) > HEIGHT)
  {
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt
  if (__h <= 0)
  {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;

  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y / 8) * SH1106_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y & 7);
  if (mod)
  {
    // mask off the high n bits we want to set
    mod = 8 - mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE};
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if (h < mod)
    {
      mask &= (0XFF >> (mod - h));
    }

    switch (color)
    {
    case WHITE:
      *pBuf |= mask;
      break;
    case BLACK:
      *pBuf &= ~mask;
      break;
    case INVERSE:
      *pBuf ^= mask;
      break;
    }

    // fast exit if we're done here!
    if (h < mod)
    {
      return;
    }

    h -= mod;

    pBuf += SH1106_LCDWIDTH;
  }

  // write solid bytes while we can - effectively doing 8 rows at a time
  if (h >= 8)
  {
    if (color == INVERSE)
    { // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do
      {
        *pBuf = ~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while (h >= 8);
    }
    else
    {
      // store a local value to work with
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do
      {
        // write our value in
        *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while (h >= 8);
    }
  }

  // now do the final partial byte, if necessary
  if (h)
  {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
    register uint8_t mask = postmask[mod];
    switch (color)
    {
    case WHITE:
      *pBuf |= mask;
      break;
    case BLACK:
      *pBuf &= ~mask;
      break;
    case INVERSE:
      *pBuf ^= mask;
      break;
    }
  }
}
void Adafruit_SH1106::clearRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
  for (int16_t i = x; i < x + w; i++)
  {
    drawFastVLineInternal(i, y, h, 0);
  }
}