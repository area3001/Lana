#include "debug.h"
#include "lana.h"

typedef enum {FALSE = 0, TRUE = 1 } boolean;

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


#define RED 90,0,0
#define GREEN 0,90,0
#define BLUE 0,0,90
#define YELLOW 45,45,0
#define WHITE 30,30,30

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

uint8_t testPair(uint8_t pinA, uint8_t pinB,  uint8_t r1,uint8_t g1,uint8_t b1, uint8_t r2,uint8_t g2,uint8_t b2, boolean test_connection)
{
    uint8_t errors = 0;

    // Test shorts to VCC
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

    // Test shorts to GND
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

    if (test_connection)
    {
        pinMode(pinA, INPUT);
        pinMode(pinB, OUTPUT);
        Delay_Ms(1);
        digitalWrite(pinB, LOW);
        Delay_Ms(1);
        if (digitalRead(pinA) > 0){
            errors += 1;
        }
        digitalWrite(pinB, HIGH);
        Delay_Ms(1);
        if (digitalRead(pinA) == 0){
            errors += 1;
        }
        Delay_Ms(1);
        digitalWrite(pinB, LOW);
        pinMode(pinB, INPUT);
        if (errors){
            SetLed(0, RED);
            SetLed(1, YELLOW);
            SetLed(2, r1,g1,b1);
            SetLed(3, r2,g2,b2);
            Write();
            return errors;
        }
    }


   SetLed(0, GREEN);
   SetLed(1, GREEN);
   SetLed(2, GREEN);
   SetLed(3, GREEN);
   Write();

    return errors;
}


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
    initNeopixel();

    runDefaultAnimation();

    for (uint8_t i=0;i<14;i++)
    {
       SetLed(i, 0,0,0);
    }
    Write();

    Delay_Ms(100);


    while (1){

        if(
           testPair(PIN_PD1, PIN_PB5, RED, RED, TRUE) ||
           testPair(PIN_PA3, PIN_PA5, RED, BLUE, TRUE) ||
           testPair(PIN_PA4, PIN_PA9, RED, YELLOW, TRUE) ||
           testPair(PIN_PA6, PIN_PA7, BLUE, RED, FALSE) ||  //TST
           testPair(PIN_PA2, PIN_PB4, BLUE, BLUE, TRUE) ||
           testPair(PIN_PB6, PIN_PB3, BLUE, YELLOW, TRUE) ||
           testPair(PIN_PB7, PIN_PA15, YELLOW, RED, TRUE) ||
           testPair(PIN_PA0, PIN_PA14, YELLOW, BLUE, TRUE) ||  //PA14 == SWC
           testPair(PIN_PA1, PIN_PA13, YELLOW, YELLOW, TRUE) ||  //PA13 == SWD
           testPair(PIN_PB0, PIN_PB1, YELLOW, WHITE, TRUE)
           ) Delay_Ms(100);
        else {
            while (1) {
                SetLed(0, GREEN);
                SetLed(1, GREEN);
                SetLed(2, GREEN);
                SetLed(3, GREEN);
                Write();
                Delay_Ms(100);
            }
        }

        Delay_Ms(100);
    }
}
