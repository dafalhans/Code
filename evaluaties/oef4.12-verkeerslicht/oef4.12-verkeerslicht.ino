// oef 4.12 Stuur  een verkeerslicht met 2 schakelaars verbonden met je Arduino. 
// S1 en S2 open => normale werking.
// S1 dicht dan oranje lamp blijft knipperen met snelheid 0.5/0.5/0.5 ……
// S2 dicht => Verkeerslicht op halve snelheid!
// Opgelet : S1 heeft voorrang op S2, dus gewoon knipperen indien beiden gesloten zijn.
// Stuur in de  eerste week na de herfstvakantie een gezipte folder via www.wetransfer.com naar wim.verlinden@cvofocus.be

// In de folder :
// 1.	verslag met min omschrijving van de opdracht
// 2.	code C++
// 3.	schema
// 4.	tijdsdiagramma verloop 2 richtingen
// 5.	kort filmpje dat de goede werking toont op je arduino uno bord.





int s1_toggle_button = 1; // orange knipperen
int s2_toggle_button = 2; // halve snelheid, 3 leds in normale cyclus normal_interval maal 2 
int leds_a[] = {5,4,3}; // 5 = rood, 4 = orande, 3 = groen  - lichtA
int leds_b[] = {8,7,6}; // 5 = rood, 4 = orande, 3 = groen  - lichtB
bool cyclus[3] = {0,0,0}; // status van de cyclus van de lichten -- linked list?


// Variables will change:
int ledState_a = LOW;  // ledState used to set the LED
int ledState_b = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

const long normal_interval = 10000; // normale snlheid 10 seconden (20 seconden bij indrukken van s2_toggle_button)
const long normal_orange_interval = 5000; // normale snlheid 10 seconden (20 seconden bij indrukken van s2_toggle_button)
// constants won't change:
const long blink_interval = 500;  // interval at which to blink (500 milliseconds)


void setup() {
    Serial.begin(115200);
    for(int i = 1; i<3; i++){
        pinMode(leds_a[i], OUTPUT);
        pinMode(leds_b[i], OUTPUT);
    }
    pinMode(s1_toggle_button,INPUT_PULLUP);
    pinMode(s2_toggle_button,INPUT_PULLUP);

    // digitalWrite(leds[1],HIGH);
    // delay(2000);
    // digitalWrite(leds[1],LOW);
    // delay(2000);

}

void loop() {
    
    static bool s1_before_state = 1;
    bool s1_new_state = digitalRead(s1_toggle_button);
    Serial.println(s1_new_state);

    static bool s2_before_state = 1;
    bool s2_new_state = digitalRead(s2_toggle_button);
    Serial.println(s2_new_state);

    unsigned long currentMillis = millis();

    if (s1_new_state != s1_before_state) {
        s1_before_state = s1_new_state;

        if(s1_before_state != 1){
            // Serial.println("should be high");
            // digitalWrite(leds[0],HIGH);

            // Orange Pinken


            // Serial.println(String(currentMillis));
            // Serial.println(String(currentMillis - previousMillis));

            if (currentMillis - previousMillis >= blink_interval) {
                // save the last time you blinked the LED
                previousMillis = currentMillis;

                // if the LED is off turn it on and vice-versa:
                if (ledState_a == LOW || leds_b == LOW) {
                ledState_a = HIGH;
                ledState_b = HIGH;
                } else {
                ledState_a = LOW;
                ledState_a = LOW;
                }
                digitalWrite(leds_a[1], ledState_a);
                digitalWrite(leds_b[1], ledState_b);

        }else{
            // Serial.println("appears to be low");
            // digitalWrite(leds[0],LOW);

            // Normaal verkeerslicht

            // HIER MOET DE IF voor s2_


            // start
            // led a : 1,0,0
            // led b : 0,0,1

            // na 10 seconden
            // led a : 1,0,0
            // led b : 0,1,0

            // na 5 seconden
            // led a : 0,0,1
            // led b : 1,0,0

            // na 10 seconden
            // led a : 0,1,0
            // led b : 1,0,0

            // na 5 seconden (start)
            // led a : 1,0,0
            // led b : 0,0,1



            

        


            if (currentMillis - previousMillis >= normal_interval) {
                // save the last time you blinked the LED
                previousMillis = currentMillis;

                

                // if the LED is off turn it on and vice-versa:
                if (ledState_a == LOW) {
                ledState_a = HIGH;
                ledState_b = LOW;
                } else {
                ledState_a = LOW;
                ledState_a = HIGH;
                }
                digitalWrite(leds_a[1], ledState_a);
                digitalWrite(leds_b[1], ledState_b);

        }
    // delay(20); // verhogen indien nodig
    }

        }
    }

}


  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.

//   unsigned long currentMillis = millis();
//   Serial.println(String(currentMillis));
//     Serial.println(String(currentMillis - previousMillis));

//   if (currentMillis - previousMillis >= interval) {
//     // save the last time you blinked the LED
//     previousMillis = currentMillis;

//     // if the LED is off turn it on and vice-versa:
//     if (ledState == LOW) {
//       ledState = HIGH;
//     } else {
//       ledState = LOW;
//     }

  