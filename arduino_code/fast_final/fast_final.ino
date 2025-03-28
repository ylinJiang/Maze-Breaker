#include "PinChangeInterrupt.h"    
#include <TimerOne.h>
#include <TimerFive.h>     //定时中断

#include "RgbUltrasonic.h"              //导入RGB超声波库
#include "Adafruit_NeoPixel.h"

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define IOPin  33                        //定义RGB超声波 IO引脚
#define RGBPin 32                          //定义RGB超声波 RGB引脚


Adafruit_NeoPixel rgb_display = Adafruit_NeoPixel(6, RGBPin, NEO_GRB + NEO_KHZ800);
RgbUltrasonic mRUS04(IOPin , RGBPin);             //创建RGB超声波对象与初始化IO引脚和RGB引脚

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

static const unsigned char left[] U8X8_PROGMEM ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x7F,0x00,0x00,0x00,0x00,0x00,
0x00,0xF0,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0xE0,0x7F,0x00,0x00,0xFE,0x0F,0x00,
0x00,0xC0,0xFF,0x00,0x00,0xF8,0x0F,0x00,0x00,0x80,0xFF,0x01,0x00,0xF0,0x1F,0x00,0x00,0x00,0xFF,0x01,0x00,0xE0,0x1F,0x00,0x00,0x00,0xFE,0x01,0x00,0xE0,0x1F,0x00,
0x00,0x00,0xFC,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x78,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x00,
0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x1F,0x00,
0x00,0x00,0x00,0x00,0x00,0xE0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x0F,0x00,
0x00,0x00,0x00,0x00,0xC0,0xFF,0x07,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0xE0,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0x80,0xFF,0xFF,0x7F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char right[] U8X8_PROGMEM ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x7F,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x07,0x00,
0x00,0x00,0xE0,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0x3F,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,
0x00,0x80,0xFF,0xFF,0xFF,0xFF,0x3F,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0xF0,0x7F,0x00,0x00,0xFE,0x07,0x00,
0x00,0xF0,0x1F,0x00,0x00,0xFF,0x03,0x00,0x00,0xF8,0x0F,0x00,0x80,0xFF,0x01,0x00,0x00,0xF8,0x07,0x00,0x80,0xFF,0x00,0x00,0x00,0xF8,0x07,0x00,0x80,0x7F,0x00,0x00,
0x00,0xFC,0x03,0x00,0x00,0x3F,0x00,0x00,0x00,0xFC,0x03,0x00,0x00,0x1E,0x00,0x00,0x00,0xFC,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0xFC,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x03,0x00,0x00,0x00,0x00,0x00,
0x00,0xF8,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0xE0,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x07,0x00,
0x00,0x00,0xFE,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char up[] U8X8_PROGMEM ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x07,0x00,0x00,
0x00,0x00,0xE0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,
0x00,0x00,0x00,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char car[] U8X8_PROGMEM ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xBF,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,
0xE0,0xFF,0xFE,0xFF,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,
0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0x3B,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFE,0xFF,0x3F,0x40,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x7F,0xFF,
0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x09,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0xE7,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0xF0,
0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0xE0,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0x7F,0x00,0xF0,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0xFF,0xFF,0x1F,0x00,0xF8,0x01,
0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0xF8,0x01,0x00,0x00,0x00,0x96,0xFF,0xFF,0xFF,0xFF,0x03,0x00,0xFC,0x01,0x00,0x00,0x18,0x03,0xFC,0xFF,0xFF,0xFF,
0x00,0x00,0xFC,0x01,0x00,0x00,0x1C,0x00,0xF0,0xFF,0xFF,0x1F,0x00,0x00,0xFC,0x01,0x00,0x00,0x0E,0x00,0x80,0xFF,0xFF,0x03,0x00,0x00,0xFE,0x01,0x00,0x00,0x07,0x18,
0x00,0xFE,0xFF,0x00,0x00,0x40,0xFE,0x00,0x00,0x80,0x07,0x3C,0x00,0xF9,0x3F,0x00,0x00,0x40,0xFE,0x01,0x00,0xC0,0x07,0x3C,0x00,0xF8,0x0F,0x00,0x00,0x22,0xFE,0x00,
0x00,0xC0,0x03,0x1C,0x00,0xE0,0x07,0x00,0x00,0x27,0x7E,0x00,0x00,0xE0,0x03,0x00,0x0E,0x00,0x03,0x00,0x00,0x27,0x7E,0x00,0x00,0xE0,0x03,0x00,0x1F,0x00,0x01,0x00,
0x18,0x63,0x3A,0x00,0x00,0xE0,0x03,0x00,0x1F,0x80,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0xE0,0x07,0x00,0x0E,0x00,0x00,0xF8,0x3D,0x80,0x00,0x00,0x00,0xE0,0x01,0x00,
0x00,0x00,0x00,0xFC,0x1D,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x09,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0xFE,0x01,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x20,0x00,0xFF,0x81,0x00,0x00,0x00,0x30,0x80,0x61,0x00,0x00,0x30,0x00,0xFF,0x41,0x00,0x00,0x00,0x38,0x00,0xF3,0x01,0x00,0x10,0x80,0xFF,
0x21,0x00,0x00,0x00,0x78,0x00,0xF6,0x07,0x00,0x18,0x80,0xF7,0x11,0x00,0x00,0x00,0xF0,0x01,0xFC,0x5F,0x00,0x18,0x80,0xFF,0x09,0x00,0x00,0x00,0xE0,0x07,0xE7,0xFF,
0x01,0x1C,0xC0,0xFF,0x04,0x00,0x00,0x00,0xC0,0x7F,0xB0,0xFF,0x03,0x0C,0xC0,0xFF,0x02,0x00,0x00,0x00,0x00,0x3E,0x38,0xFE,0x07,0x0C,0xC0,0xFF,0x00,0x00,0x00,0x00,
0x00,0x7C,0x00,0xF8,0x0F,0x0E,0xC0,0x7F,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0xE6,0x0F,0x0F,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,0xC0,0x03,0x86,0x0F,0x0F,0x80,0x3F,
0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x70,0x1E,0x0F,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x30,0x10,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x01,
0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0F,0xC0,0x07,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0x00,0xF8,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x2F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
};

String Target_Value;             //串口获取的速度字符串变量
int valuelf;
int valuerf;
int valuelb;
int valuerb;  
float Targetlf=0,Targetrf=0,Targetlb=0,Targetrb=0;                     //用于存储通过PI控制器计算得到的用于调整电机转速的PWM值的整形变量 


int startPWM=40;                 //初始PWM
int PWM_Restrict=215;            //startPW+PWM_Restric=255<256

/*※※※※※※超级重点※※※※※※
*所谓累计在正确的方向即
*(1)计数器方向
*(2)电机输出方向(控制电机转速方向的接线是正着接还是反着接)
*(3)PI 控制器 里面的误差(Basi)运算是目标值减当前值(Target-Encoder),还是当前值减目标值(Encoder-Target)
*三个方向只有对应上才会有效果否则你接上就是使劲的朝着一个方向(一般来说是反方向)满*/

const int trigPin2 = 48; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 47; // Echo Pin of Ultrasonic Sensor

const int trigPin3 = 44; // Trigger Pin of Ultrasonic Sensor
const int echoPin3 = 45; // Echo Pin of Ultrasonic Sensor
 
// Encoder output to Arduino Interrupt pin// 编码器的A相位接Arduino的D2.
int encoder_A_LF= 18; 
int encoder_B_LF=31;

int encoder_A_RF= 19; 
int encoder_B_RF=38;

int encoder_A_LB = 3;              //编码器A相引脚
int encoder_B_LB = 49;

int encoder_A_RB = 2;
int encoder_B_RB = A1;

// Pulse count from encoder // 用来储存脉冲数量
volatile long encoderCountlf = 0;
volatile long encoderCountrf = 0;
volatile long encoderCountlb = 0;
volatile long encoderCountrb = 0;

volatile long code60 = 0;
volatile long code60rf = 0;

//maze
int maze[13][13];//create a maze
int x = 11;//direction on row
int y = 1;//direction on col
int dir[4] = {1,2,3,4};//分别代表左下右上，左转就顺序调用元素，右转逆序调用
int counter = 2;//初始化方向为右,counter为下标
int corner[50][2];//用于存储转角y
int dir_record[50];//用于储存转角方向
int count = 0;//用于记录corner
int lost_x=11; 
int lost_y=0;
int future_x=11;
int future_y=0;


 void ISR_EncoderA_LF() {
// Phase A: Low to High // A相位从低到高
  if (digitalRead(encoder_A_LF) == HIGH) {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_LF) == LOW) {
      encoderCountlf--;
      code60--; }        // CW, 顺时针  
    else {
      encoderCountlf++;
      code60++; }       // CCW， 逆时针  
  }
  else  // Phase A: High to Low // A相位从高到低 
  {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_LF) == HIGH) {
      encoderCountlf--;
      code60--; }         // CW   
    else {
      encoderCountlf++;
      code60++; }         // CCW
    }
 }


void ISR_EncoderA_RF() {
// Phase A: Low to High // A相位从低到高
  if (digitalRead(encoder_A_RF) == HIGH) {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_RF) == LOW) {
      encoderCountrf++; 
      code60rf++;
      }        // CW, 顺时针  
    else {
      encoderCountrf--;
      code60rf--; }       // CCW， 逆时针  
  }

  else  // Phase A: High to Low // A相位从高到低 
  {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_RF) == HIGH) {
      encoderCountrf++; 
       code60rf++;}         // CW   
    else {
      encoderCountrf--;
      code60rf--; }         // CCW
    }
 }


void ISR_EncoderA_LB() {
// Phase A: Low to High // A相位从低到高
  if (digitalRead(encoder_A_LB) == HIGH) {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_LB) == LOW) {
      encoderCountlb--;
     }        // CW, 顺时针  
    else {
      encoderCountlb++;
      }       // CCW， 逆时针  
  }

  else  // Phase A: High to Low // A相位从高到低 
  {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_LB) == HIGH) {
      encoderCountlb--;
       }         // CW   
    else {
      encoderCountlb++;
       }         // CCW
    }
 }


void ISR_EncoderA_RB() {
// Phase A: Low to High // A相位从低到高
  if (digitalRead(encoder_A_RB) == HIGH) {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_RB) == LOW) {
      encoderCountrb++; }        // CW, 顺时针  
    else {
      encoderCountrb--; }       // CCW， 逆时针  
  }

  else  // Phase A: High to Low // A相位从高到低 
  {
    // check phase B // 观察B相位从而判断转动方向
    if (digitalRead(encoder_B_RB) == HIGH) {
      encoderCountrb++; }         // CW   
    else {
      encoderCountrb--; }         // CCW
    }
 }


/////////PWM输出引脚////////
#define PWMLF 12
#define PWMRF 8
#define PWMLB 6
#define PWMRB 5

#define DIR1LF 34
#define DIR1RF 37
#define DIR1LB 43
#define DIR1RB A4

#define DIR2LF 35
#define DIR2RF 36
#define DIR2LB 42
#define DIR2RB A5

#define SW4 10
#define SW3 11
#define SW2 13
#define SW1 46

#define SW0 29
#define SW5 28

/**********定时器中断触发函数*********/
int Velocitylf=0;
int Velocityrf=0;
int Velocitylb=0;
int Velocityrb=0;  

void control()
{      
    Velocitylf=encoderCountlf;    //把采用周期(内部定时中断周期)所累计的脉冲上升沿和下降沿的个数,赋值给速度
    Velocityrf=encoderCountrf;    //把采用周期(内部定时中断周期)所累计的脉冲上升沿和下降沿的个数,赋值给速度
    Velocitylb=encoderCountlb;    //把采用周期(内部定时中断周期)所累计的脉冲上升沿和下降沿的个数,赋值给速度
    Velocityrb=encoderCountrb;    //把采用周期(内部定时中断周期)所累计的脉冲上升沿和下降沿的个数,赋值给速度
    
    encoderCountrb = 0;           //并清零
    valuerb =Incremental_PI_RB(Velocityrb,Targetrb);  //通过目标值和当前值在这个函数下算出我们需要调整用的PWM值
    SetPWMB_B(valuerb);    //将算好的值输出给电机        

   
    encoderCountrf=0;           //并清零
    valuerf=Incremental_PI_RF(Velocityrf,Targetrf);  //通过目标值和当前值在这个函数下算出我们需要调整用的PWM值
    SetPWMB_F(valuerf);    //将算好的值输出给电机       

    encoderCountlb = 0;           //并清零
    valuelb=Incremental_PI_LB(Velocitylb,Targetlb);  //通过目标值和当前值在这个函数下算出我们需要调整用的PWM值
    SetPWMA_B(valuelb);    //将算好的值输出给电机     
    
     encoderCountlf=0;           //并清零
    valuelf=Incremental_PI_LF(Velocitylf,Targetlf);  //通过目标值和当前值在这个函数下算出我们需要调整用的PWM值
    SetPWMA_F(valuelf);    //将算好的值输出给电机 

              
}

/***********PI控制器****************/


int Incremental_PI_LF (int Encoder,int Target)
{  
  static float Biaslf,PWMlf,Last_biaslf,Last2_biaslf;
   Biaslf=Target-Encoder;                                  //计算偏差,目标值减去当前值
   PWMlf+=6.5*(Biaslf-Last_biaslf)+1*Biaslf+0.06*(Biaslf+Last2_biaslf-2*Last_biaslf);   //增量式PI控制计算

   if(PWMlf>PWM_Restrict)
   PWMlf=PWM_Restrict;                                     //限幅
   
   if(PWMlf<-PWM_Restrict)
   PWMlf=-PWM_Restrict;                                    //限幅  
   
   Last2_biaslf=Last_biaslf;
   Last_biaslf=Biaslf;                                //保存上一次偏差 
   return PWMlf;                                           //增量输出
}

int Incremental_PI_RF (int Encoder,int Target)
{  
   static float Biasrf,PWMrf,Last_biasrf,Last2_biasrf;
   Biasrf=Target-Encoder;                                  //计算偏差,目标值减去当前值
   PWMrf+=3.5*(Biasrf-Last_biasrf)+1*Biasrf+1.2*(Biasrf+Last2_biasrf-2*Last_biasrf);   //增量式PI控制计算
   
   if(PWMrf>PWM_Restrict)
   PWMrf=PWM_Restrict;                                     //限幅
   
   if(PWMrf<-PWM_Restrict)
   PWMrf=-PWM_Restrict;  
    
   Last2_biasrf=Last_biasrf; 
   Last_biasrf=Biasrf;                                //保存上一次偏差 
   return PWMrf;                                           //增量输出
}

int Incremental_PI_LB (int Encoder,int Target)
{  
  static float Biaslb,PWMlb,Last_biaslb,Last2_biaslb;
   Biaslb=Target-Encoder;                                  //计算偏差,目标值减去当前值
   PWMlb+=7.5*(Biaslb-Last_biaslb)+1*Biaslb+0.05*(Biaslb+Last2_biaslb-2*Last_biaslb);   //增量式PI控制计算
   
   if(PWMlb>PWM_Restrict)
   PWMlb=PWM_Restrict;                                     //限幅
   
   if(PWMlb<-PWM_Restrict)
   PWMlb=-PWM_Restrict;                                    //限幅  
   
   Last2_biaslb=Last_biaslb;
   Last_biaslb=Biaslb;                             //保存上一次偏差 
   return PWMlb;                                      //增量输出
}

int Incremental_PI_RB (int Encoder,int Target)
{  
  static float Biasrb,PWMrb,Last_biasrb,Last2_biasrb;
   Biasrb=Target-Encoder;                                  //计算偏差,目标值减去当前值
   PWMrb+=7.5*(Biasrb-Last_biasrb)+1*Biasrb+0.05*(Biasrb+Last2_biasrb-2*Last_biasrb);   //增量式PI控制计
   
   if(PWMrb>PWM_Restrict)
   PWMrb=PWM_Restrict;                                     //限幅
   
   if(PWMrb<-PWM_Restrict)
   PWMrb=-PWM_Restrict;                                    //限幅  
   
   Last2_biasrb=Last_biasrb;
   Last_biasrb=Biasrb;                                //保存上一次偏差 
   return PWMrb;                                           //增量输出
}


long microsecondsToCentimeters(long microseconds) {
  //speed of sound wave divided by 2 (go and back)
   return microseconds * 0.034 / 2; 
}

//设置电机A的PWM值，范围是-255 ~ 255，其中0~255对应占空比 0~100，负数表示电机反转
void SetPWMA_F(int pwm)
{  
  
  if(pwm>0)
  {
    analogWrite(PWMLF,pwm+startPWM);
    digitalWrite(DIR2LF,HIGH);
    digitalWrite(DIR1LF,LOW);
  }
  else if(pwm<0)
  {
    analogWrite(PWMLF,-pwm+startPWM);
    digitalWrite(DIR1LF,HIGH);
    digitalWrite(DIR2LF,LOW);
  }
  else
  {
    analogWrite(PWMLF,0);
  }    
}   


//设置电机B的PWM值，范围是-255 ~ 255，其中0~255对应占空比 0~100，负数表示电机反转
void SetPWMB_F(int pwm)
{
  if(pwm>0)
  {
    analogWrite(PWMRF,pwm+startPWM);
    digitalWrite(DIR1RF,HIGH);
    digitalWrite(DIR2RF,LOW);
  }
  else if(pwm<0)
  {
    analogWrite(PWMRF,-pwm+startPWM);
    digitalWrite(DIR2RF,HIGH);
    digitalWrite(DIR1RF,LOW);
  }
  else
  {
    analogWrite(PWMRF,0);
  }

}

//设置电机A的PWM值，范围是-255 ~ 255，其中0~255对应占空比 0~100，负数表示电机反转
void SetPWMA_B(int pwm)
{
  if(pwm>0)
  {
    analogWrite(PWMLB,pwm+startPWM);
    digitalWrite(DIR2LB,HIGH);
    digitalWrite(DIR1LB,LOW);
  }
  else if(pwm<0)
  {
    analogWrite(PWMLB,-pwm+startPWM);
    digitalWrite(DIR1LB,HIGH);
    digitalWrite(DIR2LB,LOW);
  }
  else
  {
    analogWrite(PWMLB,0);
  }

}

//设置电机B的PWM值，范围是-255 ~ 255，其中0~255对应占空比 0~100，负数表示电机反转
void SetPWMB_B(int pwm)
{
  if(pwm>0)
  {
    digitalWrite(DIR1RB,HIGH);
    digitalWrite(DIR2RB,LOW);
    analogWrite(PWMRB,pwm+startPWM);
  }
  else if(pwm<0)
  {
    analogWrite(PWMRB,-pwm+startPWM);
    digitalWrite(DIR2RB,HIGH);
    digitalWrite(DIR1RB,LOW);
  }
  else
  {
    analogWrite(PWMRB,0);
  }
}

unsigned long Time_Echo_us = 0;

int getsuperwave1 ()
{
  int cm = mRUS04.GetUltrasonicDistance();
  return cm;   
}

int getsuperwave2 ()
{
  long duration, inches, cm;
   digitalWrite(trigPin2, LOW);//clears the trigger pin condition
   delayMicroseconds(2);
   digitalWrite(trigPin2, HIGH);
   //sets the trigger pin HIGH (ACTIVE) for 10 us
   delayMicroseconds(10);
   digitalWrite(trigPin2, LOW);   
   duration = pulseIn(echoPin2, HIGH);
   // us to cm
   cm = microsecondsToCentimeters(duration);
   return cm;
}

int getsuperwave3 ()
{
  long duration, inches, cm;
   digitalWrite(trigPin3, LOW);//clears the trigger pin condition
   delayMicroseconds(2);
   digitalWrite(trigPin3, HIGH);
   //sets the trigger pin HIGH (ACTIVE) for 10 us
   delayMicroseconds(10);
   digitalWrite(trigPin3, LOW);   
   duration = pulseIn(echoPin3, HIGH);
   // us to cm
   cm = microsecondsToCentimeters(duration);
   return cm;
}

void move (int fl,int fr,int bl,int br)
{
 Targetlf=fl;
 Targetrf=fr;
 Targetlb=bl;
 Targetrb=br; 
}

//*************************************************
//***************************************************
//值大约在0-36，不要超过这个范围！！！！！！！！！！！！！！
void stop()
{
  move(0,0,0,0);
}

void turnright()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,right);
  // u8g2.sendBuffer();
  
  code60=0;
  move(15,0,15,0);
  while(code60<1485)
  {
   delay(2); 
  }
  code60=0;
  move(0,0,0,0);
  counter--;
}

void turnright_fast()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,right);
  // u8g2.sendBuffer();
  
  code60=0;
  move(30,-30,30,-30);
  while(code60<780)
  {
   delay(2); 
  }
  code60=0;
  move(0,0,0,0);
  counter--;
}

void turnleft()
{ 
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,left);
  // u8g2.sendBuffer();

  code60rf=0;
  move(-3,15,-5,15);
  while(code60rf<1400)
  {
    delay(2);
  }
  code60rf=0;
  move(0,0,0,0);
  // delay(400);
  counter++;
}

void turnleft_fast()
{ 
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,left);
  // u8g2.sendBuffer();

  code60rf=0;
  move(-30,30,-30,30);
  while(code60rf<780)
  {
    delay(2);
  }
  code60rf=0;
  move(0,0,0,0);
  // delay(400);
  counter++;
}

void turnback()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,right);
  // u8g2.sendBuffer();

  code60=0;
  move(15,-13,15,-13);
  while(code60<1520)
  {
    delay(2); 
  }
  code60=0;
  move(0,0,0,0);
  // delay(400);
  counter=counter+2;
}

// void turnback_fast()
// {
//   // u8g2.clearBuffer();
//   // u8g2.drawXBMP(32,0, 64,64,right);
//   // u8g2.sendBuffer();

//   code60=0;
//   move(30,-30,30,-30);
//   while(code60<1400)
//   {
//     delay(2); 
//   }
//   code60=0;
//   move(0,0,0,0);
//   // delay(400);
//   counter=counter+2;
// }


void forward_60()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,up);
  // u8g2.sendBuffer();

  code60=0;
  move(15,15,15,15);
  while(code60<2200)
  {
    if((digitalRead(SW1)==0)&(digitalRead(SW3)==1))
    {
      move(11,15,11,15);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==0))
    {
      move(15,11,15,11);
    }else if((digitalRead(SW1)==0)&(digitalRead(SW3)==0))
    {
      if((digitalRead(SW2)==0)||(digitalRead(SW4)==0))
      {
        delay(4);
        if(((digitalRead(SW2)==0)||(digitalRead(SW4)==0))&&(code60>200))
        {
          break;
        }
      }            
       move(15,15,15,15);
    }
    else if((digitalRead(SW1)==1)&(digitalRead(SW3)==1))
    {
      if(digitalRead(SW2)==0)
      {
        move(5,15,5,15);
      }
      if(digitalRead(SW4)==0)
      {
        move(15,5,15,5);
      }
      if((digitalRead(SW2)==1)&(digitalRead(SW4)==1))
      {
        if(digitalRead(SW0)==1)
      {
        move(5,15,5,15);
         delay(10);
      }
      if(digitalRead(SW5)==1)
      {
        move(15,5,15,5);
         delay(10);
      }
      }
    }
  }

  move(0,0,0,0);
  code60=0;
  
  //方向是dir[corner%4]
  if(dir[counter%4] == 1){//walk on left
    y= y-1;
    maze[x][y] = 1;
    y= y-1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 2){//walk downward
    x = x+1;
    maze[x][y] = 1;
    x = x+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 3){//walk on right
    y = y+1;
    maze[x][y] = 1;
    y = y+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 4){//walk upward
    x = x-1;
    maze[x][y] = 1;
    x = x-1;
    maze[x][y] = 1;
  } 
}

void forward_medium()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,up);
  // u8g2.sendBuffer();

  code60=0;
  move(20,20,20,20);
  while(code60<2200)
  {
    if((digitalRead(SW1)==0)&(digitalRead(SW3)==1))
    {
      move(16,20,16,20);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==0))
    {
      move(20,16,20,16);
    }else if((digitalRead(SW1)==0)&(digitalRead(SW3)==0))
    {
      if((digitalRead(SW2)==0)||(digitalRead(SW4)==0))
      {
        delay(4);
        if(((digitalRead(SW2)==0)||(digitalRead(SW4)==0))&&(code60>200))
        {
          break;
        }
      }            
       move(20,20,20,20);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==1))
    {
      if(digitalRead(SW2)==0)
      {
        move(10,20,10,20);
      }
      if(digitalRead(SW4)==0)
      {
        move(20,10,20,10);
      }
      if((digitalRead(SW2)==1)&(digitalRead(SW4)==1))
      {
        if(digitalRead(SW0)==1)
      {
        move(5,20,5,20);
        delay(10);
      }
      if(digitalRead(SW5)==1)
      {
        move(20,5,20,5);
        delay(10);
      }
      }
    }
  }

  move(0,0,0,0);
  code60=0;
  
  //方向是dir[corner%4]
  if(dir[counter%4] == 1){//walk on left
    y= y-1;
    maze[x][y] = 1;
    y= y-1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 2){//walk downward
    x = x+1;
    maze[x][y] = 1;
    x = x+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 3){//walk on right
    y = y+1;
    maze[x][y] = 1;
    y = y+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 4){//walk upward
    x = x-1;
    maze[x][y] = 1;
    x = x-1;
    maze[x][y] = 1;
  } 
}
  

  void forward_38()
{
  // u8g2.clearBuffer();
  // u8g2.drawXBMP(32,0, 64,64,up);
  // u8g2.sendBuffer();

  code60=0;
  move(15,15,15,15);
  while(code60<1650)
  {
    if((digitalRead(SW1)==0)&(digitalRead(SW3)==1))
    {
      move(11,15,11,15);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==0))
    {
      move(15,11,15,11);
    }else if((digitalRead(SW1)==0)&(digitalRead(SW3)==0))
    {
      if((digitalRead(SW2)==0)||(digitalRead(SW4)==0))
      {
        delay(4);
        if(((digitalRead(SW2)==0)||(digitalRead(SW4)==0))&&(code60>200))
        {
          break;
        }
      }            
       move(15,15,15,15);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==1))
    {
      if(digitalRead(SW2)==0)
      {
        move(5,15,5,15);
      }
      if(digitalRead(SW4)==0)
      {
        move(15,5,15,5);
      }
      if((digitalRead(SW2)==1)&(digitalRead(SW4)==1))
      {
        if(digitalRead(SW0)==1)
      {
        move(5,15,5,15);
        delay(20);
      }
      if(digitalRead(SW5)==1)
      {
        move(15,5,15,5);
        delay(20);
      }
      }
    }
  }

  move(0,0,0,0);
  code60=0;
  
  //方向是dir[corner%4]
  if(dir[counter%4] == 1){//walk on left
    y= y-1;
    maze[x][y] = 1;
    y= y-1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 2){//walk downward
    x = x+1;
    maze[x][y] = 1;
    x = x+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 3){//walk on right
    y = y+1;
    maze[x][y] = 1;
    y = y+1;
    maze[x][y] = 1;
  }else if(dir[counter%4] == 4){//walk upward
    x = x-1;
    maze[x][y] = 1;
    x = x-1;
    maze[x][y] = 1;
  } 
}

void reMaper(){

  bool stop = true;
  int count = 0;
  
  while(stop == true){
    stop = false;
    count++;
    for (int i = 1; i < 12; i++) {
      for (int j = 1; j < 12; j++) {
        if (maze[i][j] == 1 && ((maze[i][j - 1] == 0 && maze[i][j + 1] == 0 && maze[i - 1][j] == 0 && maze[i + 1][j] == 1) || (maze[i][j - 1] == 0 && maze[i][j + 1] == 1 && maze[i - 1][j] == 0 && maze[i + 1][j] == 0) || (maze[i][j - 1] == 1 && maze[i][j + 1] == 0 && maze[i - 1][j] == 0 && maze[i + 1][j] == 0) || (maze[i][j - 1] == 0 && maze[i][j + 1] == 0 && maze[i - 1][j] == 1 && maze[i + 1][j] == 0))) {
          maze[i][j] = 0;
          stop = true;
        }
      }
    }
  }
  //record corner
  bool acontinue = true;
  
  int cc = 0;//记录dir_record角标，最后和couter对应数值是同一点


   x = 11;//direction on row
   y = 0;//direction on col
  
    count=0;
   while(acontinue==true) {
    //special case:start point

    if(x==11 && y==0){
      if (maze[x][y + 1] == 1) {//右
          future_y++;
          //更新信息
          lost_x = x;
          lost_y = y;
          //当前坐标自增进入下一循环
          y++;
      }
    }
    ////special case:end point
    else if(x==0 && y==11){
      acontinue=false;
    }
    else {
      if (lost_x - x != -1 && maze[x - 1][y] == 1) {//上方
        future_x--;
        if ((maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y + 1] == 1) || (maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y + 1] == 1)) {
          corner[count][0] = x;
          corner[count][1] = y;
          count++;
        }
        maze[lost_x][lost_y] = 0;
        //判断是否拐弯，用的是上一次更新的x和lost_x
        if (lost_y == y - 1) {
            dir_record[cc] = 5;//左转
            cc++;
        }
        if (lost_y == y + 1) {
            dir_record[cc] = 6;//右转
            cc++;
        }

      //更新信息
      lost_x = x;
      lost_y = y;
      //当前坐标位置更新进入下一循环
      x--;
    } else if (lost_x - x != 1 && maze[x + 1][y] == 1) {//下方
      future_x++;
      if ((maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y + 1] == 1) || (maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y + 1] == 1)) {
        corner[count][0] = x;
        corner[count][1] = y;
        count++;
      }
      
      maze[lost_x][lost_y] = 0;
      if (lost_y==y+1) {//左转
        dir_record[cc] = 5;
        cc++;
      }
      if (lost_y==y-1) {//右转
        dir_record[cc] = 6;
        cc++;
      }
      //更新信息
      lost_x = x;
      lost_y = y;
      //当前坐标位置更新进入下一循环
      x++;
    } else if (lost_y - y != -1 && maze[x][y - 1] == 1) {//左
      future_y--;
      if ((maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y + 1] == 1) || (maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y + 1] == 1)) {
        corner[count][0] = x;
        corner[count][1] = y;
        count++;
      }
    maze[lost_x][lost_y] = 0;
    if (lost_x == x - 1) {//右转
      dir_record[cc] = 6;
      cc++;
    }
    if (lost_x == x + 1) {//左转
      dir_record[cc] = 5;
      cc++;
    }
    //更新信息
    lost_x = x;
    lost_y = y;
    //当前坐标位置更新进入下一循环
    y--;
    } else if (lost_y - y != 1 && maze[x][y + 1] == 1) {//右
      future_y++;
        if ((maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y - 1] == 1) || (maze[x][y] == 1 && maze[x + 1][y] == 1 && maze[x][y + 1] == 1) || (maze[x][y] == 1 && maze[x - 1][y] == 1 && maze[x][y + 1] == 1)) {
          corner[count][0] = x;
          corner[count][1] = y;
          count++;
        }      
        maze[lost_x][lost_y] = 0;
        if (lost_x == x - 1) {
        //记录方向
          dir_record[cc] = 5;//5为左转
          cc++;
        }
        if (lost_x == x + 1) {//右转
          dir_record[cc] = 6;
          cc++;
        }
      //更新信息
        lost_x = x;
        lost_y = y;
      //当前坐标自增进入下一循环
        y++;
      } 
      else {
        acontinue = false;
      }
    }
  }


  //reset all the maze elements to 0
  for (int e = 0; e < 13; e++) {
    for (int f = 0; f < 13; f++) {
      maze[e][f] =0;
    }
  }



}



float Target;

int button = 30;

void setup() {
  // put your setup code here, to run once:
  //PWM引脚
  noInterrupts();

  rgb_display.begin();
  Serial.begin(9600);
  
  rgb_display.setPixelColor(0, 0xff0000);   // 红灯
  rgb_display.setPixelColor(1, 0xff0000);   // 红灯
  rgb_display.setPixelColor(2, 0xff0000);   // 红灯
  rgb_display.setPixelColor(3, 0xff0000);   // 红灯
  rgb_display.setPixelColor(4, 0xff0000);   // 红灯
  rgb_display.setPixelColor(5, 0xff0000);   // 红灯
  rgb_display.show();
  delay(100);
  rgb_display.setPixelColor(0, 0x00ff00);   // 绿灯
  rgb_display.setPixelColor(1, 0x00ff00);   // 绿灯
  rgb_display.setPixelColor(2, 0x00ff00);   // 绿灯
  rgb_display.setPixelColor(3, 0x00ff00);   // 绿灯
  rgb_display.setPixelColor(4, 0x00ff00);   // 绿灯
  rgb_display.setPixelColor(5, 0x00ff00);   // 绿灯
  rgb_display.show();
  delay(100);
  rgb_display.setPixelColor(0, 0xFFFF00);   // 黄灯
  rgb_display.setPixelColor(1, 0xFFFF00);   // 蓝灯
  rgb_display.setPixelColor(2, 0xFFFF00);   // 蓝灯
  rgb_display.setPixelColor(3, 0xFFFF00);   // 蓝灯
  rgb_display.setPixelColor(4, 0xFFFF00);   // 蓝灯
  rgb_display.setPixelColor(5, 0xFFFF00);   // 蓝灯
  rgb_display.show();
  delay(100);
  rgb_display.setPixelColor(0, 0);   // 黄灯
  rgb_display.setPixelColor(1, 0);   // 蓝灯
  rgb_display.setPixelColor(2, 0);   // 蓝灯
  rgb_display.setPixelColor(3, 0);   // 蓝灯
  rgb_display.setPixelColor(4, 0);   // 蓝灯
  rgb_display.setPixelColor(5, 0);   // 蓝灯
  rgb_display.show();
  delay(100);

  u8g2.begin();

  pinMode(button, INPUT_PULLUP);

   pinMode(trigPin2, OUTPUT);
   pinMode(echoPin2, INPUT);  
   pinMode(trigPin3, OUTPUT);
   pinMode(echoPin3, INPUT);  

  pinMode(PWMLF, OUTPUT);
  pinMode(PWMRF, OUTPUT);
  pinMode(PWMLB, OUTPUT);
  pinMode(PWMRB, OUTPUT);

  pinMode(DIR1LF, OUTPUT);
  pinMode(DIR1RF, OUTPUT);
  pinMode(DIR1LB, OUTPUT);
  pinMode(DIR1RB, OUTPUT);
  
  pinMode(DIR2LF, OUTPUT);
  pinMode(DIR2RF, OUTPUT);
  pinMode(DIR2LB, OUTPUT);
  pinMode(DIR2RB, OUTPUT);

  //编码器引

  pinMode(encoder_A_LF, INPUT);
  pinMode(encoder_B_LF, INPUT);
  pinMode(encoder_A_RF, INPUT);
  pinMode(encoder_B_RF, INPUT);
  pinMode(encoder_A_LB, INPUT);
  pinMode(encoder_B_LB, INPUT);
  pinMode(encoder_A_RB, INPUT);
  pinMode(encoder_B_RB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder_A_LF), ISR_EncoderA_LF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_A_RF), ISR_EncoderA_RF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_A_LB), ISR_EncoderA_LB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder_A_RB), ISR_EncoderA_RB, CHANGE);

  pinMode(SW3, INPUT); //将巡线传感器连接的数字引脚设置为输入模式
  pinMode(SW4, INPUT); //将巡线传感器连接的数字引脚设置为输入模式
  pinMode(SW2, INPUT); //将巡线传感器连接的数字引脚设置为输入模式
  pinMode(SW1, INPUT); //将巡线传感器连接的数字引脚设置为输入模式
  
  
  Timer5.initialize(10000);
  Timer5.attachInterrupt(control); // blinkLED to run every 0.15 seconds

  interrupts();

  maze[11][0] = 1;
  maze[0][11] = 1;
  maze[11][1] = 1;

corner[0][0]=11;
corner[0][1]=3;
corner[1][0]=7;
corner[1][1]=3;
corner[2][0]=7;
corner[2][1]=5;
corner[3][0]=9;
corner[3][1]=5;
corner[4][0]=9;
corner[4][1]=9;
corner[5][0]=7;
corner[5][1]=9;
corner[6][0]=7;
corner[6][1]=7;
corner[7][0]=5;
corner[7][1]=7;
corner[8][0]=5;
corner[8][1]=1;
corner[9][0]=1;
corner[9][1]=1;
corner[10][0]=1;
corner[10][1]=11;


dir_record[0]=5;
dir_record[1]=6;
dir_record[2]=6;
dir_record[3]=5;
dir_record[4]=5;
dir_record[5]=5;
dir_record[6]=6;
dir_record[7]=5;
dir_record[8]=6;
dir_record[9]=6;
dir_record[10]=5;


}

int bottoncount = 0;
int track_x = 0;
int track_y = 0;
int track_cnt = 0;

void loop() {   

//   //判断按钮的按动次数
  if( digitalRead(button) == LOW)
  { 
    delay(100);
    if(digitalRead(button) == LOW){
      //画图
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_cu12_tr);		  
      u8g2.setCursor(45,20);
      u8g2.print("Please");
      u8g2.setCursor(40,40);
      u8g2.print("Release");
      u8g2.setCursor(25,60);
      u8g2.print("the Button");
      u8g2.sendBuffer();
      delay(1000);
      if(digitalRead(button) == HIGH)
      {
        bottoncount++;
        //画图
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_cu12_tr);		  
      u8g2.setCursor(40,40);
      u8g2.print("Succeed");
      u8g2.sendBuffer();
      Serial.println(bottoncount);
      }
    }
  }
  
 
   
   if(bottoncount%6 == 1)
   {
     code60=0;
  move(15,15,15,15);
  while(code60<2200)
  {
    if((digitalRead(SW1)==0)&(digitalRead(SW3)==1))
    {
      move(11,15,11,15);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==0))
    {
      move(15,11,15,11);
    }else if((digitalRead(SW1)==0)&(digitalRead(SW3)==0))
    {
      if((digitalRead(SW2)==0)||(digitalRead(SW4)==0))
      {
        delay(4);
        if(((digitalRead(SW2)==0)||(digitalRead(SW4)==0))&&(code60>200))
        {
          break;
        }
      }            
       move(15,15,15,15);
    }else if((digitalRead(SW1)==1)&(digitalRead(SW3)==1))
    {
      if(digitalRead(SW2)==0)
      {
        move(5,15,5,15);
      }
      if(digitalRead(SW4)==0)
      {
        move(15,5,15,5);
      }
      if((digitalRead(SW2)==1)&(digitalRead(SW4)==1))
      {
        if(digitalRead(SW0)==1)
      {
        move(5,15,5,15);
        delay(20);
      }
      if(digitalRead(SW5)==1)
      {
        move(15,5,15,5);
        delay(20);
      }
      }
    }
  }

  move(0,0,0,0);
  code60=0;

  bottoncount++;
   }
   
  
    else if(bottoncount%6 == 2)
  {

    if(dir[counter%4] == 1){//walk on left
    track_x = x;
    track_y = y-2;
    }else if(dir[counter%4] == 2){//walk downward
    track_x = x+2;
    track_y = y;
    }else if(dir[counter%4] == 3){//walk on right
    track_x = x;
    track_y = y+2;
    }else if(dir[counter%4] == 4){//walk upward
    track_x = x-2;
    track_y = y;
    }

    forward_60();


    for(int i=0;i<50;i++)
    {
      if(corner[i][0] == track_x)
      {
        if(corner[i][1] == track_y)
        {
            if(dir_record[i] == 5)
            {
              turnleft();
            }
            if(dir_record[i] == 6)
            {
              turnright();
            }
        }
      }
    }

  }


  // {
  //   if(dir[counter%4] == 1){//walk on left
  //   track_x = x;
  //   track_y = y-2;
  //   }else if(dir[counter%4] == 2){//walk downward
  //   track_x = x+2;
  //   track_y = y;
  //   }else if(dir[counter%4] == 3){//walk on right
  //   track_x = x;
  //   track_y = y+2;
  //   }else if(dir[counter%4] == 4){//walk upward
  //   track_x = x-2;
  //   track_y = y;
  //   }

  //   forward_60();
  //   if((x < 1)&&(track_y == 11))
  //   {
  //     u8g2.clearBuffer();
  //     u8g2.setFont(u8g2_font_cu12_tr);		  
  //     u8g2.setCursor(0,20);
  //     u8g2.print("Mission");
  //     u8g2.setCursor(0,40);
  //     u8g2.print("Accomplished");
  //     u8g2.setCursor(0,60);
  //     u8g2.print("Happy!");
  //     u8g2.sendBuffer();
       
  //     bottoncount++;
  //   }

//     for(int i=0;i<50;i++)
//     { 
//        if(corner[i][0] == 0)
//        {
//          track_cnt = i;
//          break;
//        }
//     }

//     for(int i=0;i<track_cnt;i++)
//     {
//       if(corner[i][0] == track_x)
//       {
//         if(corner[i][1] == track_y)
//         {
//             if(dir_record[i] == 5)
//             {
//               turnleft();
//             }else{
//               turnright();
//             }
//         }
//       }
//     }
//   }




  
  //    if(bottoncount%4 == 1)
  // {

  // u8g2.clearBuffer();
  // u8g2.setFont(u8g2_font_cu12_tr);		  
  // u8g2.setCursor(20,40);
  // u8g2.print(x);
  // u8g2.setCursor(40,40);
  // u8g2.print(y);
  // u8g2.sendBuffer();

  // // if((x == 1)&&(y == 11))
  // //   {
  // //     move(0,0,0,0);
  // //     bottoncount++;
      
  // //   }

  //   if(dir[counter%4] == 1){//walk on left
  //   track_x = x;
  //   track_y = y-2;
  //   }else if(dir[counter%4] == 2){//walk downward
  //   track_x = x+2;
  //   track_y = y;
  //   }else if(dir[counter%4] == 3){//walk on right
  //   track_x = x;
  //   track_y = y+2;
  //   }else if(dir[counter%4] == 4){//walk upward
  //   track_x = x-2;
  //   track_y = y;
  //   }

  //   forward_60();
  //   delay(200);
    
  //   for(int i=0;i<11;i++)
  //   {
  //     if(corner[i][0] == track_x)
  //     {
  //       if(corner[i][1] == track_y)
  //       {
  //           Serial.println(i);
  //           Serial.println(dir_record[i]);
  //           if(dir_record[i] == 5)
  //           {
  //             turnleft();
  //           }
  //           if(dir_record[i] == 6)
  //           {
  //             turnright();
  //           }
  //       }
  //     }
  //   }

  //    if((x == 1)&&(track_y == 11))
  //   {
  //     u8g2.clearBuffer();
  //     u8g2.setFont(u8g2_font_cu12_tr);		  
  //     u8g2.setCursor(0,20);
  //     u8g2.print("Mission");
  //     u8g2.setCursor(0,40);
  //     u8g2.print("Accomplished");
  //     u8g2.setCursor(0,60);
  //     u8g2.print("Happy!");
  //     u8g2.sendBuffer();
       
  //     bottoncount++;
  //   } 
  // }
  
  

 
//   }else if(bottoncount%4 == 2)
//   {
//     reMaper();
//     while(digitalRead(button) == HIGH)
//     {
//     }
//   }else if(bottoncount%4 == 3)
//   {
//     u8g2.clearBuffer();
//     u8g2.drawXBMP(32,0, 64,64,right);
//     u8g2.sendBuffer();  
//   }else if(bottoncount%4 == 0)
//   {
//     reMaper();
//     while(digitalRead(button) == HIGH)
//     {
//     }
//   }  


//   if(bottoncount%4 == 1)
//   {
    
    
   

//   }else if(bottoncount%4 == 2)
//   {
//     reMaper();
//     while(digitalRead(button) == HIGH)
//     {
//     }
//   }else if(bottoncount%4 == 3)
//   {
    // if(dir[counter%4] == 1){//walk on left
    // track_x = x;
    // track_y = y-2;
    // }else if(dir[counter%4] == 2){//walk downward
    // track_x = x+2;
    // track_y = y;
    // }else if(dir[counter%4] == 3){//walk on right
    // track_x = x;
    // track_y = y+2;
    // }else if(dir[counter%4] == 4){//walk upward
    // track_x = x-2;
    // track_y = y;
    // }

    // forward_60();
    // if((x < 1)&&(track_y == 11))
    // {
    //   u8g2.clearBuffer();
    //   u8g2.setFont(u8g2_font_cu12_tr);		  
    //   u8g2.setCursor(0,20);
    //   u8g2.print("Mission");
    //   u8g2.setCursor(0,40);
    //   u8g2.print("Accomplished");
    //   u8g2.setCursor(0,60);
    //   u8g2.print("Happy!");
    //   u8g2.sendBuffer();
       
    //   bottoncount++;
    // }

    // for(int i=0;i<50;i++)
    // { 
    //    if(corner[i][0] == 0)
    //    {
    //      track_cnt = i;
    //      break;
    //    }
    // }

    // for(int i=0;i<track_cnt;i++)
    // {
    //   if(corner[i][0] == track_x)
    //   {
    //     if(corner[i][1] == track_y)
    //     {
    //         if(dir_record[i] == 5)
    //         {
    //           turnleft();
    //         }else{
    //           turnright();
    //         }
    //     }
    //   }
    // }
//   }else if(bottoncount%4 == 0)
//   {
//     reMaper();
//     while(digitalRead(button) == HIGH)
//     {
//     }
//   }  
  // forward_60();
  // delay(1000);
  // turnback();
  // delay(1000);

  //   while(Serial.available()>0)          //检测串口是否接收到了数据
  // {
  //   Target_Value=Serial.readString();  //读取串口字符串
  //   Target=Target_Value.toFloat();     //将字符串转换为浮点型,并将其赋给目标值
  //   move(Target,Target,Target,Target);
  // }
   
  // Serial.print("目标转速频率:");      //串口打印出设定的目标转速
  //   Serial.println(Target);
  // Serial.print("实际转速频率lf:");   
  //   Serial.println(Velocitylf);     
  // Serial.print("实际转速频率rf:");   
  //   Serial.println(Velocityrf);
  // Serial.print("实际转速频率lb:");   
  //   Serial.println(Velocitylb);
  // Serial.print("实际转速频率rb:");   
  //   Serial.println(Velocityrb);  
  //   Serial.println(""); 
  
  // delay(200);
  //  forward_60();
 
  // Serial.print("sw1   ");
  // Serial.println(digitalRead(SW1));

  // Serial.print("sw3   ");
  // Serial.println(digitalRead(SW3));

  // Serial.print("sw2   ");
  // Serial.println(digitalRead(SW2));

  // Serial.print("sw4   ");
  // Serial.println(digitalRead(SW4));

  // Serial.print("sw0   ");
  // Serial.println(digitalRead(SW0));

  // Serial.print("sw5   ");
  // Serial.println(digitalRead(SW5));
  // delay(200);

  // Serial.println("");
  

    // Serial.print("superwave1   ");
    // Serial.println(getsuperwave1());

    // Serial.print("superwave2   ");
    // Serial.println(getsuperwave2());

    //  Serial.print("supermave3   ");
    // Serial.println(getsuperwave3());

  //   // Serial.print("sw4   ");
  //   // Serial.println(digitalRead(SW4));

    // Serial.println("");
  
    // delay(500); 

  // // reMaper();
 


// if(getsuperwave2()>50)
//   {
//     turnleft();
//     forward_38();
//   }else
//   {
//     if(getsuperwave1()>40)
//     {
//       //做了一个修正
//       if((digitalRead(SW1)==0)&(digitalRead(SW3)==0)&((digitalRead(SW2)==0)||(digitalRead(SW4)==0)))
//       {
//         code60=0;
//         move(15,15,15,15);
//         while(code60<50)
//         {
//           Serial.println(code60);  
//         }
//         code60=0;
//         move(0,0,0,0);
//       }

//       forward_60();
//     }else
//     {
//       if(getsuperwave3()>40)
//       {
//         turnright();
//         forward_38();
//       }else
//       {
//         turnback();
//         forward_60();
//       }
//     }
//   }
//  turnright();
//  delay(500);
// // turnright();
}
 