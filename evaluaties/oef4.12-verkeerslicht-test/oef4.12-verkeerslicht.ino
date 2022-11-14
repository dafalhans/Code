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





int s1_toggle_button = 10; // orange knipperen
int s2_toggle_button = 11 ; // halve snelheid, 3 leds in normale cyclus normal_interval maal 2 
int leds_a[] = {5,4,3}; // 5 = rood, 4 = orande, 3 = groen  - lichtA
int leds_b[] = {8,7,6}; // 5 = rood, 4 = orande, 3 = groen  - lichtB


// Variables will change:
int ledState_a = LOW;  // ledState used to set the LED
int ledState_b = LOW;  // ledState used to set the LED

// keep track of various states of first Traffic Light
int trafficLightCurrentState;   // 1 = start_groen, 2 = groen, 3 = orange (licht 1), 4 = rood, 5 = orange (licht 2)

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

const unsigned long normal_interval = 5000; // normale snlheid 10 seconden (20 seconden bij indrukken van s2_toggle_button)
const unsigned long normal_orange_interval = 1500; // normale snlheid 10 seconden (20 seconden bij indrukken van s2_toggle_button)
// constants won't change:
const unsigned long blink_interval = 500;  // interval at which to blink (500 milliseconds)

//unsigned long startTheTime;
unsigned long calculated_interval;
unsigned long starttijd;

void setup() {


    //bug ?
    //Serial.begin(115200);
    for(int i = 0; i<3; i++){
        pinMode(leds_a[i], OUTPUT);
        pinMode(leds_b[i], OUTPUT);
    }
    pinMode(s1_toggle_button,INPUT_PULLUP);
    pinMode(s2_toggle_button,INPUT_PULLUP);


    // setup check to see if all my leds go on... (like, is my wiring and stuff OK)
    // for(int i = 0; i<3; i++){
    //     digitalWrite(leds_a[i], HIGH);
    //     digitalWrite(leds_b[i], HIGH);
    // }
    // delay(2000);
    // for(int i = 0; i<3; i++){
    //     digitalWrite(leds_a[i], LOW);
    //     digitalWrite(leds_b[i], LOW);
    // }

    //startTheTime = millis() * 1000;
    //startTheTime = millis();

}

void loop() {
    // static bool knipperen = false;
    // static bool traag_verspringen = false;
    // static bool normaal_verspringen = false;
    bool knipperen = false;
    bool traag_verspringen = false;
    bool normaal_verspringen = false;

    bool s1_before_state = 1;
    bool s1_new_state = digitalRead(s1_toggle_button);
    Serial.print("s1 before state: ");Serial.println(s1_before_state);
    Serial.print("s1 new    state: ");Serial.println(s1_new_state);

    bool s2_before_state = 1;
    bool s2_new_state = digitalRead(s2_toggle_button);
    Serial.print("s2 before state: ");Serial.println(s2_before_state);
    Serial.print("s2 new    state: ");Serial.println(s2_new_state);

    //delay(1000);

    // need this for the LED timing to be non blocking
    //unsigned long currentMillis = millis() * 1000;
    unsigned long currentMillis = millis();
   

    if (s1_new_state != s1_before_state) {
        s1_before_state = s1_new_state;

        if(s1_before_state != 1){
            knipperen = true;
        }else{
            knipperen = false;
             digitalWrite(leds_a[1], LOW);digitalWrite(leds_b[1], LOW); // ensure orange off on both lights when button no longer pressed
        }
    }
    else if(s2_new_state != s2_before_state) {
        s2_before_state = s2_new_state;

        if(s2_before_state != 1){
            traag_verspringen = true;
        }else{
            traag_verspringen = false;
        }
    }else{
        normaal_verspringen = true;
    }



    if(knipperen){
        digitalWrite(leds_a[2], LOW);digitalWrite(leds_a[0], LOW); // GREEN 0, RED 1 on first traffic light
        digitalWrite(leds_b[2], LOW);digitalWrite(leds_b[0], LOW); // GREEN 0, RED 1 on second traffic light
        Serial.println("Orange pinken");
            if (currentMillis - previousMillis >= blink_interval) {
                // save the last time you blinked the LED
                previousMillis = currentMillis;

                // if the LED is off turn it on and vice-versa:
                if (ledState_a == LOW) {
                ledState_a = HIGH;
                } else {
                ledState_a = LOW;
                }
                // same for LED in traffic light 2
                if (ledState_b == LOW) {
                ledState_b = HIGH;
                } else {
                ledState_b = LOW;
                }

               
                digitalWrite(leds_a[1], ledState_a);
                digitalWrite(leds_b[1], ledState_b);
                
            }
    }else if(normaal_verspringen or traag_verspringen){
            if(traag_verspringen){
                calculated_interval = normal_interval * 2;
            }else{
                calculated_interval = normal_interval;
            }

        if(trafficLightCurrentState == 2)
        {

            if(starttijd == 0){
               starttijd = millis();
            }

             if (currentMillis - starttijd <= calculated_interval) {
                digitalWrite(leds_a[2], HIGH);digitalWrite(leds_a[1], LOW);digitalWrite(leds_a[0], LOW); // GREEN 1, ORANGE 0, RED 0 on first traffic light
                digitalWrite(leds_b[2], LOW);digitalWrite(leds_b[1], LOW);digitalWrite(leds_b[0], HIGH); // GREEN 0, ORANGE 0, RED 1 on second traffic light
            }else{
                trafficLightCurrentState = 3;
                starttijd = 0;
            }

        }
        else if(trafficLightCurrentState == 3)
        {
            if(starttijd == 0){
               starttijd = millis();
            }

            if(traag_verspringen){
                calculated_interval = normal_orange_interval * 2;
            }else{
                calculated_interval = normal_orange_interval;
            }

            if (currentMillis - starttijd <= calculated_interval) {
                digitalWrite(leds_a[2], LOW);digitalWrite(leds_a[1], HIGH);digitalWrite(leds_a[0], LOW); // GREEN 0, ORANGE 1, RED 0 on first traffic light
                digitalWrite(leds_b[2], LOW);digitalWrite(leds_b[1], LOW);digitalWrite(leds_b[0], HIGH); // GREEN 0, ORANGE 0, RED 1 on second traffic light
            }else{
                trafficLightCurrentState = 4;
                starttijd = 0;
            }

        }
        else if(trafficLightCurrentState == 4)
        {
            if(starttijd == 0){
               starttijd = millis();
            }
            if (currentMillis - starttijd <= calculated_interval) {
                digitalWrite(leds_a[2], LOW);digitalWrite(leds_a[1], LOW);digitalWrite(leds_a[0], HIGH); // GREEN 0, ORANGE 0, RED 1 on first traffic light
                digitalWrite(leds_b[2], HIGH);digitalWrite(leds_b[1], LOW);digitalWrite(leds_b[0], LOW); // GREEN 1, ORANGE 0, RED 0 on second traffic light
            }else{
                trafficLightCurrentState = 5;
                starttijd = 0;
            }
        }else if(trafficLightCurrentState == 5)
        {
            if(starttijd == 0){
               starttijd = millis();
            }

            if(traag_verspringen){
                calculated_interval = normal_orange_interval * 2;
            }else{
                calculated_interval = normal_orange_interval;
            }

            if (currentMillis - starttijd <= calculated_interval) {
                digitalWrite(leds_a[2], LOW);digitalWrite(leds_a[1], LOW);digitalWrite(leds_a[0], HIGH); // GREEN 0, ORANGE 1, RED 0 on first traffic light
                digitalWrite(leds_b[2], LOW);digitalWrite(leds_b[1], HIGH);digitalWrite(leds_b[0], LOW); // GREEN 0, ORANGE 0, RED 1 on second traffic light
            }else{
                trafficLightCurrentState = 2;
                starttijd = 0;
            }
        }
        else{
            trafficLightCurrentState = 2;
            starttijd = 0;
        }

    }

}


