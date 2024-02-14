#include "debug.h"
#include "lana.h"

static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }

uint32_t Wheel(uint8_t WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

uint8_t Red(uint32_t color)
   {
       return (color >> 16) & 0xFF;
   }

uint8_t Green(uint32_t color)
   {
       return (color >> 8) & 0xFF;
   }

uint8_t Blue(uint32_t color)
   {
       return color & 0xFF;
   }


#define RED 30,0,0
#define GREEN 0,30,0
#define BLUE 0,0,30
#define YELLOW 15,15,0
#define WHITE 10,10,10

uint8_t testShortToVCC(uint8_t pin)
{
    //PB6 PB7 have external pull-up's they cant be tested for shorts to vcc
    //PB3 and PA15 are connected to these pins on the test fixture and should also be skipped
    if (pin == PIN_PB6 || pin == PIN_PB7 || pin == PIN_PA15 || pin == PIN_PB3) return 0;
    uint8_t errors = 0;
    pinMode(pin, INPUT_PULLDOWN);
    if (digitalRead(pin) != 0) errors++;
    pinMode(pin, INPUT);
    return errors;
}

uint8_t testShortToGND(uint8_t pin)
{
    uint8_t errors = 0;
    pinMode(pin, INPUT_PULLUP);
    if (digitalRead(pin) == 0) errors++;
    pinMode(pin, INPUT);
    return errors;
}

uint8_t testPair(uint8_t pinA, uint8_t pinB,  uint8_t r1,uint8_t g1,uint8_t b1, uint8_t r2,uint8_t g2,uint8_t b2)
{
    uint8_t errors = 0;

    errors += testShortToVCC(pinA);
    errors += testShortToVCC(pinB);
    if (errors){
        SetLed(0, RED);
        SetLed(1, RED);
        SetLed(2, r1,g1,b1);
        SetLed(3, r2,g2,b2);
        Write();
        return errors;
    }

    errors += testShortToGND(pinA);
    errors += testShortToGND(pinB);

    if (errors){
       SetLed(0, RED);
       SetLed(1, BLUE);
       SetLed(2, r1,g1,b1);
       SetLed(3, r2,g2,b2);
       Write();
       return errors;
    }
    // Since we're here both pins are not shorted.
    // Now make one high then low and read the same state on the other pin

    //pinA = outPP
    //pinB = in
    //pinA.high
    //assert B == high
    //test all but(A,B, PULLDOWN, == LOW)
    //pinA.low
    //assert B == LOW
    //test all but(A,B, PULLUP, == HIGH)

       SetLed(0, GREEN);
       SetLed(1, GREEN);
       SetLed(2, GREEN);
       SetLed(3, GREEN);
       Write();

    return errors;
}

uint8_t all_pins[] = {
    PIN_PD1, PIN_PB5,
    PIN_PA3, PIN_PA5,
    PIN_PA4, PIN_PA9,
    PIN_PA6, PIN_PA7,   /* connected to button to GND */
    PIN_PA2, PIN_PB4,
    PIN_PB6, PIN_PB3,
    PIN_PB7, PIN_PA15,
    PIN_PA0, PIN_PA14,
    PIN_PA1, PIN_PA13,
    PIN_PB0, PIN_PB1};

uint8_t all_pin_len = sizeof(all_pins);

/*
 * Run a animated RGB wheel to show the board is working.
 * Stop when both PA6 and PA7 are shored to ground. (Test button in the test fixture is connected to these pins)
 */
void runDefaultAnimation()
{
    pinMode(PIN_PA6, INPUT_PULLUP);
    pinMode(PIN_PA7, INPUT_PULLUP);
    uint8_t offset =0;
    while(digitalRead(PIN_PA6) != 0 &&digitalRead(PIN_PA7) != 0)
    {
       offset+=10;
       for (uint8_t i=0;i<14;i++)
       {
           uint32_t c = Wheel(i*10+offset);
           SetLed(i, Red(c), Green(c), Blue(c));
       }
       Write();
       Delay_Ms(33);
    }
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    initNeopixel(); //TODO: change to PinMode(...

    runDefaultAnimation();

    for (uint8_t i=0;i<14;i++)
    {
       SetLed(i, 0,0,0);
    }
    Write();

    Delay_Ms(100);

    while (1){
        if(testPair(PIN_PD1, PIN_PB5, RED, RED) ||
           testPair(PIN_PA3, PIN_PA5, RED, BLUE) ||
           testPair(PIN_PA4, PIN_PA9, RED, YELLOW) ||
           testPair(PIN_PA6, PIN_PA7, BLUE, RED) ||
           testPair(PIN_PA2, PIN_PB4, BLUE, BLUE) ||
           testPair(PIN_PB6, PIN_PB3, BLUE, YELLOW) ||
           testPair(PIN_PB7, PIN_PA15, YELLOW, RED) ||
           testPair(PIN_PA0, PIN_PA14, YELLOW, BLUE) ||
           testPair(PIN_PA1, PIN_PA13, YELLOW, YELLOW) ||
           testPair(PIN_PB0, PIN_PB1, YELLOW, WHITE) ||
           0
           ) Delay_Ms(1000);

        Delay_Ms(100);
    }
}
