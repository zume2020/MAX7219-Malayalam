# MAX7219-Malayalam

Cascaded Malayalam Scrolling text display using MAX7219 dot matrix and Arduino.  
Displays മലയാളം (Malayayalam, a native Indian language) in LED matrix display.

[Here is an awesome MAX72XX font editor :fire:](https://pjrp.github.io/MDParolaFontEditor)

## Hardware
* Two cascaded MAX7219 dot matrix module.
* Arduino or ESP8266 NodeMCU.
* Wires

## Wiring
Connections for Arduino/ESP8266 hardware SPI

| MAX72XX | Arduino | ESP
|:-------:|:-------:|:----:
| Vcc     | 5V      | 3V3
| GND     | GND     | GND 
| DIN     | 11      | D7 
| CS      | 10      | D8 
| CLK     | 13      | D5

## Credits
This is a modified version of [MajicDesigns -- MD_Parola](https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_Double_Height_v2) Double Height V2.
