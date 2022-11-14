// https://www.arduino.cc/en/Reference/LiquidCrystal geeft overzicht van alle methodes vh object LiquidCrystal

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
//LiquidCrystal lcd(RS, E, D4, D5, D6, D7) 
 
// 1=VSS, 2 = VDD, 3 = VO , 4= RS, 5=RW , 6=D0, 7=D1 , 8=D2 , 9=D3 , 10=D4 , 11=D5 , 12=D6 , 13=D7 , 14=D8 , 15=A , 16=K 
// RW aan GND ( enkel schrijven naar LCD, lezen niet nodig)


// void setup(){
//   lcd.begin(16, 2);
// }

// void loop() {

//   // set the cursor to (0,0):

//   lcd.setCursor(0, 0);

//   // print from 0 to 9:

//   for (int thisChar = 0; thisChar < 10; thisChar++) {

//     lcd.print(thisChar);

//     delay(500);

//   }

//   // set the cursor to (16,1):

//   lcd.setCursor(16, 1);

//   // set the display to automatically scroll:

//   lcd.autoscroll();

//   // print from 0 to 9:

//   for (int thisChar = 0; thisChar < 10; thisChar++) {

//     lcd.print(thisChar);

//     delay(500);

//   }

//   // turn off automatic scrolling

//   lcd.noAutoscroll();

//   // clear screen for the next loop:

//   lcd.clear();
// }


void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("WELCOME IN CVO-");

  // go to column 0,row 1, note that this is indexed from 0
  lcd.setCursor(0, 1);
  lcd.print ("FOCUS ARDUINO");
}

void loop() {
  // put your main code here, to run repeatedly:
}

// /*
//   LCD Pin               Potentiometer Pin       UNO Pin
//   1 VSS/GND and 16  K         1                   GND
//   2 VDD/VCC and 15  A         3                   3.3V/5V
//   3 V0                        2                   Signal pin
//   4 RS                                            2
//   5 RW                                            GND
//   6 E                                             3
//   7  D0 unused
//   8  D1 unused
//   9  D2 unused
//   10 D3 unused
//   11 D4                                           4
//   12 D5                                           5
//   13 D6                                           6
//   14 D7                                           7
// */
