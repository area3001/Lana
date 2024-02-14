1) start WS2812 animation. (fast Rainbow ove 51 leds)
2) configure 2 BUTTON pins as INPUT PULLUP.
3) When BOTH button pins are LOW: stop animation, start testing routine
4) repeat testing routing n times (OK of one of the succeeds)
   1) loop over all testable pairs
       a) configure as pullup, read high
	   b) configure as pulldown, read low
	   c) configure one as output high, read other high
	   d) configure one as output low, read low
	   e) reset config for SWD
	   
# How to test
1) put the board in the test fixture like in the picture.   
_Note: be sure that the board has the EOL firmware._
2) 👀 The 3 LEDS should be black (off)
3) 👀 The LED on the board should show RGB colors alternatively
4) To start the test press and release the button,
_All tests will run and the result will be shown on the LEDS_
5) 👀 If all LEDS remain black there is an issue with pins **PA6 / PA7**
6) 👀 Check the color code on the LEDS with the error code table.
	   
# Error codes

Pins are connected and  tested in pairs. We test if pairs are shorted to ground, vcc and if their connection to the pad of the PCB works. If all tests for all pairs works you see 3 green LEDS 🟢🟢🟢. If you see any other color then the board has an issue. 

Some examples
* 🔴🔴🟡 This means that one or both of the pins PA4/PA9 ar shorted to VCC
* 🟡🔵⚪ There is no connection between pins PB7 and PB15.

| LEDS    | Meaning                    |
| ------- | -------------------------- |
| 🟢🟢🟢 | ALL Tests OK without error |
| 🔴🔴🔴 | (PD1, PB5) Short VCC       |
| 🔴🔴🔵 | (PA3, PA5) Short VCC       |
| 🔴🔴🟡 | (PA4, PA9) Short VCC       |
| 🔴🔵🔴 | (PA6, PA7) Short VCC       |
| 🔴🔵🔵 | (PA2, PB4) Short VCC       |
| 🔴🔵🟡 | (PB6, PB3) Short VCC       | 
| 🔴🟡🔴 | (PB7, PA15) Short VCC      |
| 🔴🟡🔵 | (PA0, SWCLK) Short VCC     |
| 🔴🟡🟡 | (PA1, SWD) Short VCC       |
| 🔴🟡⚪ | (PB0, PB1) Short VCC       |
| 🔵🔴🔴 | (PD1, PB5) Short GND       |
| 🔵🔴🔵 | (PA3, PA5) Short GND       |
| 🔵🔴🟡 | (PA4, PA9) Short GND       |
| 🔵🔵🔴 | (PA6, PA7) Short GND       |
| 🔵🔵🔵 | (PA2, PD4) Short GND       |
| 🔵🔵🟡 | (PB6 !pu, PB3) Short GND       | 
| 🔵🟡🔴 | (PB7 !pu, PA15) Short GND      |
| 🔵🟡🔵 | (PA0, SWCLK) Short GND     |
| 🔵🟡🟡 | (PA1, SWD) Short GND       |
| 🔵🟡⚪ | (PB0, PB1) Short GND       |
| 🟡🔴🔴 | (PD1, PB5) Short GND       |
| 🟡🔴🔵 | (PA3, PA5) Short GND       |
| 🟡🔴🟡 | (PA4, PA9) Short GND       |
| 🟡🔵🔴 | (PA6, PA7) Short GND       |
| 🟡🔵🔵 | (PA2, PD4) Short GND       |
| 🟡🔵🟡 | (PB6, PB3) Short GND       | 
| 🟡🟡🔴 | (PB7, PA15) Short GND      |
| 🟡🟡🔵 | (PA0, SWCLK) Short GND     |
| 🟡🟡🟡 | (PA1, SWD) Short GND       |
| 🟡🟡⚪ | (PB0, PB1) Short GND       |


