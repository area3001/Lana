# LANA EOL TEST v 1.0
> for latest version of this document visit: https://github.com/area3001/Lana/blob/main/EndOfLine/EOL_testing_lana.pdf

# How to test
1) put the board in the test fixture like in the picture.   
   ![Insert LANA](Media/Insert_LANA.jpg)
2) Put on the cover
   ![Cover LANA](Media/Cover_LANA.jpg)
3) Press the cover and the black test button
   ![Test LANA](Media/Test_LANA.jpg)
   1) If all LEDS are green the test was OK
   2) If leds are not GREEN check the error codes below
      ![Error LANA](Media/Error_LANA.jpg)
	  In this example we get code YELLOW RED RED. This means the PA3 or PA5 pin is not well connected

	   
# Error codes

Pins are connected and  tested in pairs. We test if pairs are shorted to ground, vcc if their connection to the pad of the PCB works and if there are internal shorts between pins. If all tests for all pairs works you see 3 green LEDS 🟢🟢🟢. If you see any other color then the board has an issue. 

| LEDS | Meaning |
| --- | --- |
| ![](Media/GGG.png) | ALL Tests OK without error |
| ![](Media/RRR.png) | Short VCC |
| ![](Media/RRB.png) | Short GND |
| ![](Media/YRR.png) | (PA3, PA5) Not connected |
| ![](Media/YRB.png) | (PA6, PB7)  Not connected |
| ![](Media/YRY.png) | (PA2, PB6) Not connected |
| ![](Media/YBR.png) | (PB0, PB1) Not connected |
| ![](Media/YBB.png) | (PA0, PA14) Not connected |
| ![](Media/YBY.png) | (PA1, PA13) Not connected |
| ![](Media/BBB.png) | Shorts between pins |


