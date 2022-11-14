// https://www.arduino.cc/en/Reference/LiquidCrystal geeft overzicht van alle methodes vh object LiquidCrystal

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
int pins[] = {8,9,10,11};
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
  Serial.begin(115200);
  for(int i = 0;i<4;i++){
    pinMode(pins[i], INPUT_PULLUP);
  }
  lcd.begin(16, 2);
//   lcd.clear();
//   lcd.print("WELCOME IN CVO-");

  // go to column 0,row 1, note that this is indexed from 0
//   lcd.setCursor(0, 1);
//   lcd.print ("FOCUS ARDUINO");
}

void loop() {
  static long counter1 = 0;
  static long counter2 = 0;
  static bool previous_value[] = {1,1,1,1};
  bool s_value;

  for (int i = 0; i < 4 ; i++){
    s_value = digitalRead(pins[i]);

    if(s_value != previous_value[i]){
        previous_value[i] = s_value;

        if(s_value == 0){

            if(i == 0){
                counter1 += 1;
            }else if(i == 1){
                counter2 += 1;
            }else if(i == 2){
                counter1 = 0;
            }else if(i == 3){
                counter2 = 0;
            }

        Serial.print("Counter 1: "); Serial.println(counter1);
        Serial.print("Counter 2: "); Serial.println(counter2);  
        }
    }

    lcd.clear();
    lcd.print("Counter 1: " + String(counter1));

    // go to column 0,row 1, note that this is indexed from 0
    lcd.setCursor(0, 1);
    lcd.print("Counter 2: " + String(counter2));

    delay(20);

  }
}