#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"

// Turn debugging on and off
#define DEBUG 1

#if DEBUG
#define PRINTS(s)   { Serial.print(F(s)); }
#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }
#else
#define PRINTS(s)
#define PRINT(s, v)
#endif

// Define the main direction for scrolling double height.
// if 1, scroll left; if 0, scroll right
#define SCROLL_LEFT 1

// Hardware adaptation parameters for scrolling
bool invertUpperZone = false;
textEffect_t scrollUpper, scrollLower;

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers may not work with your hardware and may need changing

#define NUM_ZONES 2
#define ZONE_SIZE 4
#define MAX_DEVICES (NUM_ZONES * ZONE_SIZE)

#define ZONE_UPPER  1
#define ZONE_LOWER  0

#define PAUSE_TIME 0
#define SCROLL_SPEED 50

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char *msgL[] =
{
  "123)45",
};
char *msgH; // allocated memory in setup()

void setup(void)
{
  uint8_t max = 0;

  uint8_t intens = 0;

#if DEBUG
  Serial.begin(57600);
  PRINTS("\n[Double_Height_v2]");
#endif

  // set up global parameters
  invertUpperZone = (HARDWARE_TYPE == MD_MAX72XX::GENERIC_HW || HARDWARE_TYPE == MD_MAX72XX::PAROLA_HW);
  if (invertUpperZone)
  {
#if SCROLL_LEFT // invert and scroll left
    scrollUpper = PA_SCROLL_RIGHT;
    scrollLower = PA_SCROLL_LEFT;
#else           // invert and scroll right
    scrollUpper = PA_SCROLL_LEFT;
    scrollLower = PA_SCROLL_RIGHT;
#endif
  }
  else // not invert
  {
#if SCROLL_LEFT // not invert and scroll left
    scrollUpper = PA_SCROLL_LEFT;
    scrollLower = PA_SCROLL_LEFT;
#else           // not invert and scroll right
    scrollUpper = PA_SCROLL_RIGHT;
    scrollLower = PA_SCROLL_RIGHT;
#endif
  }

  // work out the size of buffer required
  for (uint8_t i = 0; i < ARRAY_SIZE(msgL); i++)
    if (strlen(msgL[i]) > max) max = strlen(msgL[i]);

  msgH = (char *)malloc(sizeof(char) * (max + 2));

  // initialise the LED display
  P.begin(NUM_ZONES);

  // Set up zones for 2 halves of the display


  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES - 1);
  P.setIntensity(intens);
  P.setFont(BigFont);
  P.setCharSpacing(P.getCharSpacing() * 2); // double height --> double spacing
  if (invertUpperZone)
  {
    P.setZoneEffect(ZONE_UPPER, true, PA_FLIP_UD);
    P.setZoneEffect(ZONE_UPPER, true, PA_FLIP_LR);
  }
  PRINT("\nFLIP_UD=", P.getZoneEffect(ZONE_UPPER, PA_FLIP_UD));
  PRINT("\nFLIP_LR=", P.getZoneEffect(ZONE_UPPER, PA_FLIP_LR));
  PRINT("\nSCROLL_LEFT=", SCROLL_LEFT);
}

void createHString(char *pH, const char *pL)
{
  for (; *pL != '\0'; pL++)
    *pH++ = *pL | 0x80;   // offset character

  *pH = '\0'; // terminate the string
}

void loop(void)
{
  static uint8_t cycle = 0;

  P.displayAnimate();

  if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER))
  {
    PRINT("\n", cycle);
    PRINT(": ", msgL[cycle]);

    // set up the string
    createHString(msgH, msgL[cycle]);

    P.displayClear();
    if (invertUpperZone)
    {
      P.displayZoneText(ZONE_UPPER, msgH, PA_CENTER, SCROLL_SPEED, PAUSE_TIME, scrollUpper, scrollLower);
      P.displayZoneText(ZONE_LOWER, msgL[cycle], PA_CENTER, SCROLL_SPEED, PAUSE_TIME, scrollUpper, scrollLower);
    }
    else
    {
      P.displayZoneText(ZONE_LOWER, msgL[cycle], PA_LEFT, SCROLL_SPEED, PAUSE_TIME, PA_SCROLL_LEFT, PA_SCROLL_LEFT );
      P.displayZoneText(ZONE_UPPER, msgH, PA_LEFT, SCROLL_SPEED, PAUSE_TIME, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }

    // prepare for next pass
    cycle = (cycle + 1) % ARRAY_SIZE(msgL);

    // synchronize the start and run the display
    P.synchZoneStart();
  }
}
