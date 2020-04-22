#include "FastLED.h"

/**
 * K12 KITT Scanner
 * Copyright (C) 2020 Julius Clasen
 * Licensed under GNU AGPLv3
 * (See LICENSE for full license)
 */

//Debug
//#define DEBUG

//Pin definitions
#define P_ROT_0    8
#define P_ROT_1    9
#define P_ROT_2    2
#define P_ROT_3    3
#define P_ROT_4    4
#define P_ROT_5    5
#define P_ROT_6    6
#define P_ROT_7    7

#define P_SW      A3

#define P_LED_R   A1
#define P_LED_G   A2

#define P_WS2812B 11

//We're using 17 LEDs
#define NUM_LEDS 17

//Mode definitions
char mode;
#define M_KITT      0
#define M_KARR      1
#define M_RGB       2
#define M_WARN      3
#define M_HEADLIGHT 4
#define M_ECO       5
#define M_SPECIAL_A 6
#define M_SPECIAL_B 7

char animationDelay;
#define D_NORMAL 50
#define D_FAST   25

CRGB leds[NUM_LEDS];

void setup()
{
  //Pin setup
  pinMode(P_ROT_0, INPUT_PULLUP);
  pinMode(P_ROT_1, INPUT_PULLUP);
  pinMode(P_ROT_2, INPUT_PULLUP);
  pinMode(P_ROT_3, INPUT_PULLUP);
  pinMode(P_ROT_4, INPUT_PULLUP);
  pinMode(P_ROT_5, INPUT_PULLUP);
  pinMode(P_ROT_6, INPUT_PULLUP);
  pinMode(P_ROT_7, INPUT_PULLUP);
  pinMode(P_SW, INPUT_PULLUP);

  pinMode(P_LED_R, OUTPUT);
  pinMode(P_LED_G, OUTPUT);

  //WS2812B Setup
  FastLED.addLeds<NEOPIXEL, P_WS2812B>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);

//  mode = M_KITT;
  animationDelay = D_NORMAL;
  digitalWrite(P_LED_R, HIGH);
  digitalWrite(P_LED_G, LOW);

  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop()
{
  checkInputs();
  doAnimation();
}

void checkInputs()
{
  if(digitalRead(P_ROT_0) == LOW)
  {
    mode = M_KITT;
  }
  else if(digitalRead(P_ROT_1) == LOW)
  {
    mode = M_KARR;
  }
  else if(digitalRead(P_ROT_2) == LOW)
  {
    mode = M_RGB;
  }
  else if(digitalRead(P_ROT_3) == LOW)
  {
    mode = M_WARN;
  }
  else if(digitalRead(P_ROT_4) == LOW)
  {
    mode = M_HEADLIGHT;
  }
  else if(digitalRead(P_ROT_5) == LOW)
  {
    mode = M_ECO;
  }
  else if(digitalRead(P_ROT_6) == LOW)
  {
    mode = M_SPECIAL_B;
  }
  else if(digitalRead(P_ROT_7) == LOW)
  {
    mode = M_SPECIAL_A;
  }

  if(digitalRead(P_SW) == LOW)
  {
    if(animationDelay == D_NORMAL)
    {
      animationDelay = D_FAST;
      digitalWrite(P_LED_R, LOW);
      digitalWrite(P_LED_G, HIGH);
    }
    else
    {
      animationDelay = D_NORMAL;
      digitalWrite(P_LED_R, HIGH);
      digitalWrite(P_LED_G, LOW);
    }
    delay(100);
  }

  #ifdef DEBUG
  Serial.print((char) ('0' + mode));
  Serial.print(",");
  Serial.println((int) animationDelay);
  #endif
}

void doAnimation()
{
  switch(mode)
  {
    //Except for the color, both animations are identical
    case M_KITT:
    case M_KARR: //TODO: Fix KARR animation (middle LED is green instead of yellow)
    {
      int green = 0;
      if(mode == M_KARR)
      {
        green = 100;
      }

      for(int i = 0; i < NUM_LEDS; i++)
      {
        for(int j = 0; j <= 255; j += 51)
        {
          leds[i] = CRGB(j, (j * green) / 255, 0);
          if(i > 0)
          {
            char fadeA = 255 - (j / 2);
            leds[i - 1] = CRGB(fadeA, (fadeA * green) / 255, 0);
            if(i > 1)
            {
              char fadeB = 127 - (j / 2);
              leds[i - 2] = CRGB(fadeB, (fadeB * green) / 255, 0);
            }
          }
          FastLED.show();
          delay(animationDelay / 2);
        }
      }

      delay(animationDelay / 2);
      
      for(int i = NUM_LEDS - 1; i >= 0; i--)
      {
        for(int j = 0; j <= 255; j += 51)
        {
          leds[i] = CRGB(j, (j * green) / 255, 0);
          if(i < NUM_LEDS - 1)
          {
            char fadeA = 255 - (j / 2);
            leds[i + 1] = CRGB(fadeA, (fadeA * green) / 255, 0);
            if(i < NUM_LEDS - 2)
            {
              char fadeB = 127 - (j / 2);
              leds[i + 2] = CRGB(fadeB, (fadeB * green) / 255, 0);
            }
          }
          FastLED.show();
          delay(animationDelay / 2);
        }
      }
      
      delay(animationDelay / 2);

      break;
    }
    case M_RGB:
    {
      for(int i = 0; i < 128; i++)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(128, i, 0);
        }
        FastLED.show();
        delay(10);
      }

      for(int i = 128; i >= 0; i--)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(i, 128, 0);
        }
        FastLED.show();
        delay(10);
      }
      
      for(int i = 0; i < 128; i++)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(0, 128, i);
        }
        FastLED.show();
        delay(10);
      }
      
      for(int i = 128; i >= 0; i--)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(0, i, 128);
        }
        FastLED.show();
        delay(10);
      }
      
      for(int i = 0; i < 128; i++)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(i, 0, 128);
        }
        FastLED.show();
        delay(10);
      }
      
      for(int i = 128; i >= 0; i--)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(128, 0, i);
        }
        FastLED.show();
        delay(10);
      }
      
      break;
    }
    case M_WARN:
    {
      for(int i = 0; i < 256; i++)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(i, (i / 3) * 2, 0);
        }
        FastLED.show();
      }
      delay(animationDelay);
      
      for(int i = 255; i >= 0; i--)
      {
        for(int j = 0; j < NUM_LEDS; j++)
        {
          leds[j] = CRGB(i, (i / 3) * 2, 0);
        }
        FastLED.show();
      }
      delay(animationDelay);
      
      break;
    }
    case M_HEADLIGHT:
    {
      for(int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB(255, 255, 255);
        FastLED.show();
      }
      break;
    }
    case M_ECO:
    {
      for(int i = 0; i < NUM_LEDS / 2; i++)
      {
        leds[i] = CRGB(0, 200, 0); //LEDs 0 to 7
        leds[NUM_LEDS - 1 - i] = CRGB(0, 200, 0); //LEDs 16 to 9
        if(i > 1)
        {
          leds[i - 2] = CRGB(0, 0, 0);
          leds[NUM_LEDS + 1 - i] = CRGB(0, 0, 0);
        }
        FastLED.show();
        delay(animationDelay);
      }

      //Center LED (8) on
      leds[NUM_LEDS / 2] = CRGB(0, 200, 0);
      //+2 LEDs off
      leds[NUM_LEDS / 2 - 2] = CRGB(0, 0, 0);
      leds[NUM_LEDS / 2 + 2] = CRGB(0, 0, 0);
      FastLED.show();
      delay(animationDelay);
      //+1 LEDs off
      leds[NUM_LEDS / 2 - 1] = CRGB(0, 0, 0);
      leds[NUM_LEDS / 2 + 1] = CRGB(0, 0, 0);
      FastLED.show();
      delay(animationDelay);
      //Center LED off
      leds[NUM_LEDS / 2] = CRGB(0, 0, 0);
      FastLED.show();
      delay(animationDelay * 2);
      
      break;
    }
    case M_SPECIAL_B:
    {
      break;
    }
    case M_SPECIAL_A:
    {
      break;
    }
  }
}
