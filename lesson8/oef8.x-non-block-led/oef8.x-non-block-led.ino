
// int toggle_led = 2;
// int blinking_led = 3;
int toggle_button = 4;
int leds[] = {2,3};


// Variables will change:
int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 500;  // interval at which to blink (milliseconds)


void setup() {
    Serial.begin(115200);
    for(int i = 1; i<2; i++){
        pinMode(leds[i], OUTPUT);
    }
    pinMode(toggle_button,INPUT_PULLUP);

    // digitalWrite(leds[1],HIGH);
    // delay(2000);
    // digitalWrite(leds[1],LOW);
    // delay(2000);

}

void loop() {
    
    static bool s_before_state = 1;
    bool s_new_state = digitalRead(toggle_button);
    Serial.println(s_new_state);

    if (s_new_state != s_before_state) {
        s_before_state = s_new_state;

        if(s_before_state != 1){
            Serial.println("should be high");
            digitalWrite(leds[0],HIGH);
        }else{
            Serial.println("appears to be low");
            digitalWrite(leds[0],LOW);
        }
    // delay(20); // verhogen indien nodig
    }


  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.

  unsigned long currentMillis = millis();
  Serial.println(String(currentMillis));
    Serial.println(String(currentMillis - previousMillis));

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    Serial.println("should blink");
    digitalWrite(leds[1], ledState);
  }


    // digitalWrite(leds[1],HIGH);
    // delay(500);
    // digitalWrite(leds[1],LOW);
    // delay(500);


}
