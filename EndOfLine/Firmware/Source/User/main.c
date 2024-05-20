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


#define RED 45,0,0
#define GREEN 0,45,0
#define BLUE 0,0,45
#define YELLOW 25,20,0
#define PURPLE 22,0,23
#define WHITE 15,15,15
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))



uint8_t testPair(uint8_t pinA, uint8_t pinB)
{
    uint8_t errors = 0;

    pinMode(pinA, INPUT);
    pinMode(pinB, OUTPUT);
    Delay_Ms(1);
    digitalWrite(pinB, LOW);
    Delay_Ms(1);
    if (digitalRead(pinA) > 0) errors += 1;
    digitalWrite(pinB, HIGH);
    Delay_Ms(1);
    if (digitalRead(pinA) == 0) errors += 1;
    Delay_Ms(1);
    digitalWrite(pinB, LOW);
    pinMode(pinB, INPUT);

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

uint8_t all_pins[] = {
        PIN_PA3, PIN_PA5, PIN_PA6, PIN_PA7, PIN_PA2, PIN_PB6, PIN_PB7, PIN_PA0, PIN_PA1,
        PIN_PB0, PIN_PB1,PIN_PA4, PIN_PB5, PIN_PB4, PIN_PB3, PIN_PA15, PIN_PA9, /*PIN_PD0,*/
        PIN_PD1, PIN_PA14, PIN_PA13}; //PD0&1 are on OSC PIN; PD0 == NEOPIXEL

uint8_t pins[] = {PIN_PA3, PIN_PA5, PIN_PA6, PIN_PA7,
                  PIN_PA2, PIN_PB6, PIN_PB7, PIN_PA0, PIN_PA1, PIN_PB0, PIN_PB1};  //PB6&PB7 Active Pullup
uint8_t smd_pins[] = {PIN_PA4, PIN_PB5, PIN_PB4, PIN_PB3, PIN_PA15, PIN_PA9, /*PIN_PD0,*/ PIN_PD1 //PD0&1 are on OSC PIN; PD0 == NEOPIXEL
                      /*,PIN_PA14, PIN_PA13*/ }; //These are SWDIO & SWCLK, use with care

uint8_t vcc_short_test_pins[] = {PIN_PA3, PIN_PA5, PIN_PA7, PIN_PA0,
                                 PIN_PA1, PIN_PB0, PIN_PB1, PIN_PA4, PIN_PB5, PIN_PB4,
                                 PIN_PB3, PIN_PA15, PIN_PA9, PIN_PD1, PIN_PA14, PIN_PA13};  //REMOVED PD0, PB6+PA2,  PB7PA6

uint8_t gnd_short_test_pins[] = {PIN_PA3, PIN_PA5, PIN_PA0, PIN_PB6, PIN_PA2, PIN_PB7, PIN_PA6,
                                 PIN_PA1, PIN_PB0, PIN_PB1, PIN_PA4, PIN_PB5, PIN_PB4,
                                 PIN_PB3, PIN_PA15, PIN_PA9, PIN_PD1, PIN_PA14, PIN_PA13};  //REMOVED PD0, PA7 (GDN via Button)

void all_pins_output()
{
    for (int i = 0; i < ARRAY_SIZE(all_pins); i++) pinMode(all_pins[i], OUTPUT);
}

void all_pins_input_floating()
{
    for (int i = 0; i < ARRAY_SIZE(all_pins); i++) pinMode(all_pins[i], INPUT);
}

void all_pins_active_low()
{
    all_pins_output();
    for (int i = 0; i < ARRAY_SIZE(all_pins); i++) digitalWrite(all_pins[i], LOW);
}

void ok(void)
{
    for (uint8_t i=0;i<4;i++)
     {
        SetLed(i, GREEN);
     }
     Write();
     Delay_Ms(500);
}

void error(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t r3, uint8_t g3, uint8_t b3){
    SetLed(0, RED);
    SetLed(1, r1, g1, b1);
    SetLed(2, r2, g2, b2);
    SetLed(3, r3, g3, b3);
    Write();
    Delay_Ms(500);
}

void setup()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    initNeopixel();
}

int main(void)
{
    setup();

    runDefaultAnimation(); //until stopped by pressing the test button

    // clear all LEDS
    for (uint8_t i=0;i<14;i++)
    {
       SetLed(i, 0,0,0);
    }
    Write();

    Delay_Ms(500);

    //run tests indefinitely
    while (1)
    {
        int errors = 0;

        all_pins_input_floating();

        //test for VCC shorts
        for (int i = 0; i < ARRAY_SIZE(vcc_short_test_pins); i++){
            pinMode(vcc_short_test_pins[i], INPUT_PULLDOWN);
            if (digitalRead(vcc_short_test_pins[i]) == 1) {
                error(RED, RED, RED);
                errors += 1;
            }
            pinMode(vcc_short_test_pins[i], INPUT);
        }

        //test for GND shorts
        if (errors == 0) {
            for (int i = 0; i < ARRAY_SIZE(gnd_short_test_pins); i++){
              pinMode(gnd_short_test_pins[i], INPUT_PULLUP);
              if (digitalRead(gnd_short_test_pins[i]) == 0) {
                  error(RED, RED, BLUE);
                  errors += 1;
              }
              pinMode(gnd_short_test_pins[i], INPUT);
            }
        }


        if (errors == 0 && testPair(PIN_PA3, PIN_PA5)){
            error(YELLOW, RED, RED);
            errors += 1;
        }

        if (errors == 0 && testPair(PIN_PA6, PIN_PB7)){
            error(YELLOW, RED, BLUE);
            errors += 1;
        }

        if (errors == 0 && testPair(PIN_PA2, PIN_PB6)){
            error(YELLOW, RED, YELLOW);
            errors += 1;
        }

        if (errors == 0 && testPair(PIN_PB0, PIN_PB1)){
            error(YELLOW, BLUE, RED);
            errors += 1;
        }

        if (errors == 0 && testPair(PIN_PA0, PIN_PA14)){
            error(YELLOW, BLUE, BLUE);
            errors += 1;
        }

        if (errors == 0 && testPair(PIN_PA1, PIN_PA13)){
            error(YELLOW, BLUE, YELLOW);
            errors += 1;
        }

        //test for smd pins that are shorted to other pins
        if (errors == 0)
        {
            for (int i = 0; i < ARRAY_SIZE(smd_pins); i++)
            {
                all_pins_active_low();
                pinMode(smd_pins[i], INPUT_PULLUP);
                if (digitalRead(smd_pins[i]) == LOW)
                {
                    errors += 1;
                    error(BLUE, BLUE, BLUE);
                }
            }
        }

        //test for trough hole pins that are shorted to other pins
        if (errors == 0)
        {
            all_pins_active_low();
            pinMode(PIN_PA3, INPUT_PULLUP);
            pinMode(PIN_PA5, INPUT_PULLUP);

        }

        if (errors == 0) {
            ok();
        }
    }
}
