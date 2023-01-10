// include header files for sensors or output
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

// NeoPixel defined macros at compile time
#define PIXEL_PIN 29 // output: pin to pass data to led strip
#define NUMPIXELS 8  // number of leds on the ledstrip

// Joystick defined macros at compile time
#define VR_X A15 // Analoge input: left-right menu
#define VR_Y A14 // Analoge input: up-down menu
#define SW 31    // inpup: pull-up ->  on-off + make selection (pull-up sensor, hence normal state is HIGH/5V/1)

// Panicbutton
#define PANIC_PIN 25 // input:  pull-up -> !1 = alarm on   (pull-up sensor, hence normal state is HIGH/5V/1)

// Magnetic Sensor
#define MAGNEGIC_SENSOR_PIN 39          // input alarm on (pull-up sensor, hence normal state is HIGH/5V/1)
#define MAGNEGIC_SENSOR_PIN_NOT_USED 36 // test to see if this solves my problem

// PIR sensor
#define PIR_SENSOR_PIN 28 // input: 1 = alarm on (normal state is LOW/0V/0)

// Buzzer
#define BUZZER_PIN 22

// Make a signle LCD Obj
// LiquidCrystal lcd(RS, E, D4, D5, D6, D7)
// LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// keypad
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
uint8_t rowPins[ROWS] = {53, 51, 49, 47}; // Each row to Arduino output
uint8_t colPins[COLS] = {52, 50, 48, 46}; // Each column to Arduino input with pull_up

// make a single NeoPixel Obj
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

enum alarm_states
{
    INACTIVE,
    // SOMETHING_FUNNY,
    CONFIG,
    CONFIG_NEW_CODE,
    MONITORING,
    DELAYED_MONITORING,
    // DELAYED_ALARM,
    ALARM
};

enum alarm_states alarm_state;

// for some reason I need  this, to be able to read ENUM values
// const char *alarm_stateStr[] = {"INACTIVE", "CONFIG", "CONFIG_NEW_CODE", "MONITORING", "DELAYED_MONITORING", "ALARM"};

struct joystickinput
{
    int x;
    int y;
    uint8_t button;
};

// Denderen check (50 ms)
// const unsigned long DEBOUNCE_DELAY = 50UL;
const unsigned long DEBOUNCE_DELAY = 50UL; // between 20ms and 200ms ideally... 50ms does not debounce correct in wokwi for keypad keys

// joystick 5 seconden inhouden om alarm te activeren
const unsigned long ACTIVATE_ALARM = 2500UL;

// Delay om te wachten met aanschakelen alarm in monitoring stand
unsigned long DELAYED_MONITORING_DELAY = 10000UL;     // initiele delay 10 seconden
unsigned long DELAYED_MONITORING_DELAY_STEP = 5000UL; // verhogen/verlagen van de delay in stappen van 5 seconden

// Delay for Magnetic Contact... this isn't a clean solution imho... will have to do for now.
unsigned long DELAYED_ALARM_SENSOR = 10000UL;

// Time to blink the red led
const unsigned long ALARM_MONITORING_BLINK_TIME = 1000UL;

// Alarm state blink time
const unsigned long ALARM_STATE_BLINK_TIME = 500UL;

struct button_t
{
    const uint8_t pin;
    uint8_t state;
    uint8_t stateLast;
    uint8_t count;
    unsigned long lastDebounceTime;
};

struct button_t buttons[] = {
    {SW, HIGH, HIGH, 0, 0UL},                  // joystuck button
    {MAGNEGIC_SENSOR_PIN, HIGH, HIGH, 0, 0UL}, // magnetic contact switch
    {PANIC_PIN, HIGH, HIGH, 0, 0UL},
    // Donderen van keypad... maar werkt niet zo te zien
    // {colPins[0], HIGH, HIGH, 0, 0UL}, // colPins [COLS] = {52, 50, 48, 46};
    // {colPins[1], HIGH, HIGH, 0, 0UL},
    // {colPins[2], HIGH, HIGH, 0, 0UL},
    // {colPins[3], HIGH, HIGH, 0, 0UL},
};

// LCD Progress bar
// byte zero[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000 };
// byte one[] = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
// byte two[] = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
// byte three[] = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
// byte four[] = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
// byte five[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };

void setup()
{
    // Enable Serial communcation for Debug Output String Purpose
    Serial.begin(115200);

    alarm_state = INACTIVE;

    // set start state for the keypad
    for (int i = 0; i < 4; i++)
    {
        pinMode(rowPins[i], INPUT);
        pinMode(colPins[i], INPUT_PULLUP);
    }

    // pinMode(MAGNEGIC_SENSOR_PIN, OUTPUT);
    // digitalWrite(MAGNEGIC_SENSOR_PIN,LOW);

    pinMode(VR_X, INPUT);                       // joystick left-right
    pinMode(VR_Y, INPUT);                       // joystick up-down
    pinMode(SW, INPUT_PULLUP);                  // joystick button
    pinMode(PANIC_PIN, INPUT_PULLUP);           // panic button (!1)
    pinMode(MAGNEGIC_SENSOR_PIN, INPUT_PULLUP); // magnetic contact (!1)
    pinMode(PIR_SENSOR_PIN, INPUT);             // PIR sensor
    pinMode(BUZZER_PIN, OUTPUT);                // Buzzer output

    // Configure NeoPixel pin for output
    // pixels.clear();
    pixels.begin();

    lcd.begin(16, 2);
}

// Default variable in the bool... meaning I need to put this function here before actually using it... spent too much time
// looking at "was not declared in this scope" errors...

void enable_LCD(bool should_update_lcd = false)
{
    // static bool should_update_lcd = true;

    // Serial.println("Kom ik hier in de LCD loop");

    if (alarm_state == INACTIVE && should_update_lcd)
    {
        stuur_lcd("Druk op joystick", "om te activeren.");
        // should_update_lcd = !should_update_lcd;
    }

    if (alarm_state == CONFIG && should_update_lcd)
    {
        lcd.clear();
        stuur_lcd("# to set code", "* activate alarm");
    }

    if (alarm_state == CONFIG_NEW_CODE && should_update_lcd)
    {
        lcd.clear();
        stuur_lcd("Enter new code", "# to accept");
    }

    if (alarm_state == MONITORING)
    {
        lcd.clear();
        stuur_lcd("ALARM ACTIEF", "");
    }

    if (alarm_state == DELAYED_MONITORING && should_update_lcd)
    {
        lcd.clear();
        stuur_lcd("ALARM ACTIEF IN", "");
    }

    if (alarm_state == ALARM && should_update_lcd)
    {
        lcd.clear();
        stuur_lcd("ALARM !!!", "");
    }
}

void loop()
{

    // Serial.print("loop check status van de magnetische pin: ");
    // Serial.println(digitalRead(39));

    static bool temp_state = true;

    if (alarm_state == INACTIVE)
    {

        inactive_logic();
        if (temp_state)
        {
            enable_LCD(1);
            temp_state = !temp_state;
        }
    }
    else
    {
        active_logic();
    }
}

void inactive_logic()
{
    alarm_active_check();
    keyPadInput();
    buzzoff();
    // pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    // pixels.setPixelColor(1, pixels.Color(0, 0, 255));
    // pixels.show();
}

void active_logic()
{
    // static bool temp_state = true;

    static bool toggle_state = 1;

    // if (temp_state)
    // {
    //     Serial.println("# om alarm te activeren");
    //     pixels.setPixelColor(0, pixels.Color(0, 255, 0));
    //     pixels.setPixelColor(1, pixels.Color(0, 255, 0));
    //     pixels.show();

    //     temp_state = !temp_state;
    // }

    keyPadInput();

    show_alarm_state_led();

    if (alarm_state == DELAYED_MONITORING)
    {
        delayed_alarm();

        // logica van de joystick om meer/minder tijd voor ingebouwe vertraging bij het activeren van alarm

        joystickinput joy;
        joy = joystickInput();

        if ((joy.x == -2) || (joy.y == 2))
        {
            // meer tijd
            // Serial.println("Kom ik hier - verhogen?");
            DELAYED_MONITORING_DELAY = DELAYED_MONITORING_DELAY + DELAYED_MONITORING_DELAY_STEP;
        }
        else if ((joy.x == 2) || (joy.y == -2))
        {
            // minder tijd
            // Serial.println("Kom ik hier - verlagen?");
            DELAYED_MONITORING_DELAY = DELAYED_MONITORING_DELAY - DELAYED_MONITORING_DELAY_STEP;
        }
    }

    if (alarm_state == MONITORING)
    {
        check_PIR();
        check_panic_button();
        check_magnetic_contact();
    }

    if (alarm_state == ALARM)
    {

        if (toggle_state)
        {
            toggle_state = !toggle_state;
            enable_LCD(true); // update boodschap op de LCD
        }
    }

    buzzer_logic();
    // enable_LCD();
    //     joystickcheck();
    //     keyPadInput();
    //     check_PIR();
    //     check_panic_button();
    //    // check_magnetic_contact();
    //     alert();
}

// enable alarm logic via joystick push for 5 seconds
void alarm_active_check()
{

    static bool toggle_state = 1;

    // const unsigned long millisRef = millis();
    unsigned long millisRef; // = millis();
    static unsigned long last_different_state_millis = 0;

    joystickinput joy;
    joy = joystickInput(ACTIVATE_ALARM);
    bool new_state = bool(joy.button);

    if (new_state != toggle_state)
    {
        alarm_state = CONFIG;
        Serial.println("Switching State to CONFIG");
        toggle_state = !toggle_state;
        enable_LCD(true); // update boodschap op de LCD
    }
}

void show_alarm_state_led()
{

    static unsigned long previousMillis;
    static bool led_state = 0;

    if (alarm_state == INACTIVE)
    {

        set_all_leds_clear();

        pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        pixels.setPixelColor(1, pixels.Color(0, 255, 0));
        pixels.show();
    }
    else if (alarm_state == MONITORING)
    {

        if (previousMillis == 0)
        {
            previousMillis = millis();
        }

        if ((millis() - previousMillis) > ALARM_MONITORING_BLINK_TIME)
        {
            previousMillis = millis();

            if (led_state)
            {
                pixels.setPixelColor(0, pixels.Color(255, 0, 0));
                pixels.setPixelColor(1, pixels.Color(255, 0, 0));
                pixels.show();
                // Invert
            }
            else
            {
                pixels.setPixelColor(0, pixels.Color(0, 0, 255));
                pixels.setPixelColor(1, pixels.Color(0, 0, 255));
                pixels.show();
            }

            led_state = !led_state;
        }
    }
    else if (alarm_state == ALARM)
    {

        if (previousMillis == 0)
        {
            previousMillis = millis();
        }

        if ((millis() - previousMillis) > ALARM_STATE_BLINK_TIME)
        {
            previousMillis = millis();

            if (led_state)
            {
                for (int i = 0; i < 8; i++)
                {
                    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
                }

                pixels.show();
                // Invert
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
                }
                pixels.show();
            }

            led_state = !led_state; // Invert
        }
    }
}

void buzzer_logic()
{
    if (alarm_state == ALARM)
    {
        buzzit();
    }
    else
    {
        buzzoff();
    }
}

void buzzit()
{
    // Serial.println("Alarm gaat af");
    digitalWrite(BUZZER_PIN, HIGH);
}

void buzzoff()
{
    // Serial.println("Alarm gaat af");
    digitalWrite(BUZZER_PIN, LOW);
}

void delayed_alarm()
{

    static unsigned long previousMillis;

    static unsigned long delta_delayed_monitoring_time;                                  // variable om te LCD aan te sturen;
    static unsigned long partial_delayed_monitoring_time = DELAYED_MONITORING_DELAY / 5; // variable die de totale delay deelt door 5, om LCD te updaten in 5 steps
                                                                                         // static bool led_state = 0;

    if (previousMillis == 0)
    {
        previousMillis = millis();
        delta_delayed_monitoring_time = partial_delayed_monitoring_time;
        //    Serial.println("Kom ik in delayed logica 1");
    }

    if ((millis() - previousMillis) > delta_delayed_monitoring_time)
    {
        // Serial.println("Kom ik in delayed logica 2");
        // Serial.println(delta_delayed_monitoring_time);
        // Serial.println(partial_delayed_monitoring_time);
        lcd.clear();
        stuur_lcd("Alarm actief in:", String((DELAYED_MONITORING_DELAY - delta_delayed_monitoring_time) / 1000) + " seconden");
        delta_delayed_monitoring_time += partial_delayed_monitoring_time;
        // alarm_state = MONITORING;
        //  led_state = !led_state; // Invert
    }

    if ((millis() - previousMillis) > DELAYED_MONITORING_DELAY)
    {
        // Serial.println("Kom ik in delayed logica 3");
        previousMillis = millis();
        alarm_state = MONITORING;
        enable_LCD(true);
        previousMillis = 0; // reset this for next time we get in the DELAYED_MONITORING state;
        // led_state = !led_state; // Invert
    }
}

// void joystickcheck()
// {

//     joystickinput joy;
//     joy = joystickInput();
// }

joystickinput joystickInput()
{

    static int xPosition = 0;
    static int yPosition = 0;
    static uint8_t SW_state = 0;
    static int mapX = 0;
    static int mapY = 0;

    xPosition = analogRead(VR_X);
    yPosition = analogRead(VR_Y);
    SW_state = Debounce(&buttons[0]);

    mapX = map(xPosition / 100, 0, 10, -2, 2);
    mapY = map(yPosition / 100, 0, 10, -2, 2);

    joystickinput joyinput = {mapX, mapY, SW_state};

    return joyinput;
}

joystickinput joystickInput(unsigned long DELAY)
{

    static int xPosition = 0;
    static int yPosition = 0;
    static uint8_t SW_state = 0;
    static int mapX = 0;
    static int mapY = 0;

    xPosition = analogRead(VR_X);
    yPosition = analogRead(VR_Y);
    SW_state = Debounce(&buttons[0], DELAY);

    mapX = map(xPosition / 100, 0, 10, -2, 2);
    mapY = map(yPosition / 100, 0, 10, -2, 2);

    joystickinput joyinput = {mapX, mapY, SW_state};

    return joyinput;
}

void keyPadInput()
{

    static String code_input = "";
    static char last_char;
    static String secret_code = "AA30"; // start code
    static String place_holder_new_secret_code = "AA30";

    bool is_key_pressed = 0;

    for (int r = 0; r < 4; r++)
    {

        pinMode(rowPins[r], OUTPUT);
        digitalWrite(rowPins[r], 0);

        for (int k = 0; k < 4; k++)
        {

            //  if (Debounce(&buttons[uint8_t(k+3)]) == 0){                    // k+3 because we have borrowed the struct button to also do denderen
            //   But Debounce doesn't play nice with the outer for loop :-(

            if (digitalRead(colPins[k]) == 0)
            {

                is_key_pressed = 1;
                last_char = hexaKeys[r][k];

                // if (code_input == secret_code)
                // {
                //     Serial.println("code correct, toggle alarm_status!");
                //     code_input = "";
                // }

                if (alarm_state == CONFIG)
                {

                    if (code_input.length() < 4)
                    {
                        if (last_char != '*' && last_char != '#')
                        {
                            code_input += String(last_char);
                        }
                    }

                    if (last_char == '*')
                    {

                        alarm_state = CONFIG_NEW_CODE;
                        code_input = "";
                        enable_LCD(true); // update status on LCD
                    }
                    else if (last_char == '#')
                    {
                        Serial.println("Alarm State Activated");
                        alarm_state = DELAYED_MONITORING;
                        code_input = "";
                        enable_LCD(true); // update status on LCD
                    }
                }

                if (alarm_state == CONFIG_NEW_CODE)
                {

                    if (code_input.length() < 4)
                    {

                        if (last_char != '*' && last_char != '#')
                        {

                            code_input += String(last_char);
                        }

                        if (last_char == '*')
                        {
                            code_input = "";
                        }

                        lcd.clear();
                        stuur_lcd(code_input, "# to accept");
                    }

                    if (code_input.length() == 4)
                    {
                        if (last_char == '#')
                        {

                            Serial.println("code input = " + code_input);

                            if ((code_input == secret_code) || (code_input == place_holder_new_secret_code))
                            {
                                // alarm_state = MONITORING;
                                // TODO
                                alarm_state = DELAYED_MONITORING;
                                // delayed_alarm();
                                //  END TODO

                                enable_LCD(true);

                                secret_code = code_input;
                                code_input = "";
                            }

                            else
                            {
                                place_holder_new_secret_code = code_input;

                                code_input = "";
                                stuur_lcd("Repeat new code", "# to accept");
                            }
                        }
                    }

                    Serial.println("ingevoerde code = " + code_input);
                }

                if (alarm_state == MONITORING)
                {
                    if (code_input.length() == 0)
                    {
                        lcd.clear();
                        stuur_lcd("code# to disable", "* to reset input");
                    }

                    if (code_input.length() < 4)
                    {

                        if (last_char != '*' && last_char != '#')
                        {

                            code_input += String(last_char);
                            lcd.clear();
                            stuur_lcd(code_input, "# or * (reset)");
                        }

                        if (last_char == '*')
                        {
                            code_input = "";
                        }
                    }

                    if (code_input.length() == 4)
                    {

                        if (last_char == '#')
                        {

                            if (code_input == secret_code)
                            {
                                Serial.println("of zelfs hier????");
                                alarm_state = CONFIG;
                                enable_LCD(true);
                                code_input = "";
                            }
                            code_input = "";
                        }
                    }
                }

                if (alarm_state == DELAYED_MONITORING)
                {
                    if (code_input.length() == 0)
                    {
                        lcd.clear();
                        stuur_lcd("code# to disable", "* to reset input");
                    }

                    if (code_input.length() < 4)
                    {

                        if (last_char != '*' && last_char != '#')
                        {

                            code_input += String(last_char);
                            lcd.clear();
                            stuur_lcd(code_input, "# or * (reset)");
                        }

                        if (last_char == '*')
                        {
                            code_input = "";
                        }
                    }

                    if (code_input.length() == 4)
                    {

                        if (last_char == '#')
                        {

                            if (code_input == secret_code)
                            {
                                Serial.println("of zelfs hier????");
                                alarm_state = CONFIG;
                                enable_LCD(true);
                                code_input = "";
                            }
                            code_input = "";
                        }
                    }
                }

                if (alarm_state == ALARM)
                {

                    if (code_input.length() < 4)
                    {
                        if (last_char != '*' && last_char != '#')
                        {

                            // code_input += String(last_char);
                            Serial.println("Kom ik hier? keypad input in ALARM state");
                            code_input += String(last_char);
                            lcd.clear();
                            stuur_lcd("ALARM !!!", String(code_input));
                        }
                    }

                    // if (last_char == '*')
                    // {

                    //     alarm_state = CONFIG_NEW_CODE;
                    //     code_input = "";
                    //     enable_LCD(true); // update status on LCD
                    // }
                    else if (last_char == '#')
                    {

                        if (code_input == secret_code)
                        {
                            Serial.println("Alarm State De-Activated");
                            alarm_state = INACTIVE;
                            code_input = "";
                            enable_LCD(true);
                        }
                        else
                        {
                            code_input = "";
                        }
                    }
                }
            }

            delay(20); // adding this here for debouncing problems with keypad
        }

        digitalWrite(rowPins[r], 1);
        pinMode(rowPins[r], INPUT);
    }
}

void check_PIR()
{

    static bool pir_previous_state = 0;
    bool pir_new_state = digitalRead(PIR_SENSOR_PIN);

    Serial.print("PIR CHECK");
    Serial.println(pir_new_state);

    if (pir_previous_state != pir_new_state)
    {
        pir_previous_state = pir_new_state;

        if (1 == pir_new_state)
        {
            alarm_state = ALARM;
            Serial.println("PIR activated");

            // set pixel 5 LED
            pixels.setPixelColor(5, pixels.Color(0, 255, 0));
            pixels.show();
        }
    }
}

void check_panic_button()
{

    static bool panic_previous_state = 1;
    bool panic_new_state = Debounce(&buttons[2]);

    if (panic_previous_state != panic_new_state)
    {
        panic_previous_state = panic_new_state;

        if (0 == panic_new_state)
        {
            alarm_state = ALARM;
            Serial.println("Panic activated");

            // set pixel 6 LED
            pixels.setPixelColor(6, pixels.Color(0, 255, 0));
            pixels.show();
        }
    }
}

void check_magnetic_contact()
{

    static unsigned long previousMillis = 0;
    static bool sensor_state = 0;

    // static bool sensor_state = 1;
    bool state_pin = digitalRead(MAGNEGIC_SENSOR_PIN);
    Serial.println(state_pin);

    if (state_pin)
    {

        Serial.println(" Kom ik hier? ");

        if (sensor_state != state_pin)
        {
            previousMillis = millis();
            sensor_state = !sensor_state;

            Serial.println("En hier? ");
        }

        if (sensor_state && (millis() - previousMillis > DELAYED_ALARM_SENSOR))
        {
            // Serial.println("Alarm moet gaan.");
            // Serial.println("Alarm gaat aan");
            alarm_state = ALARM;
            Serial.println("Magnetic Panic activated");
            pixels.setPixelColor(7, pixels.Color(0, 255, 0));
            pixels.show();
            sensor_state = !sensor_state;
        }
    }

    // Serial.println("Alarm gaat aan");
    // alarm_state = ALARM;
    // Serial.println("Magnetic Panic activated");
    // pixels.setPixelColor(7, pixels.Color(0, 255, 0));
    // pixels.show();
}

// Serial.println("Magnetic Panic No activated");

void set_all_leds_clear()
{
    for (int i = 0; i < 8; i++)
    {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
    }
}

void alert()
{

    static bool previous_state = 0;

    bool new_state;

    if (alarm_state == ALARM)
    {
        new_state = 1;
    }

    if (previous_state != new_state)
    {
        stuur_lcd("Alarm Alarm", "Code to unlock");
        previous_state = new_state;
    }
}

// helper functies

void stuur_lcd(String lijn_top, String lijn_bottom)
{

    lcd.setCursor(0, 0);
    lcd.print(lijn_top);
    lcd.setCursor(0, 1);
    lcd.print(lijn_bottom);
}

// denderen knop
boolean Debounce(struct button_t *button)
{
    const unsigned long millisRef = millis(); // ophalen huidige tijd

    uint8_t state = digitalRead((*button).pin); // uitlezen PIN (niet-ingedrukt is HIGH, ingedrukt is LOW)
    if (state != (*button).stateLast)           // uitgelezen staat vergelijken met laatst gekende staat - start standaard op HIGH voor Panic knop en Joystick knop
    {
        (*button).lastDebounceTime = millisRef; // loop tijdstip millis opslaan als laatste dendercheck tijdstip
    }

    if ((millisRef - (*button).lastDebounceTime) > DEBOUNCE_DELAY) // loop tijdstip millis verminderen met laatst opgeslaan dendercheck.
    {                                                              // Indien delte groter dan 50ms, verder met de logica.  Indien te klein zou er te veel HIGH,LOW,HIGH zijn
        if (state != (*button).state)                              // als de loop uitgelzen PIN status verschild van de gekende state in de struct
        {
            (*button).state = state; // dan passen we de gekende state aan in de struct, met de nieuwe (loop) state

            if ((*button).state == LOW) // Indien de state LOW (ingedrukt bij Panic en Joystick, omwille van INPUT_PULLUP)
            {
                (*button).stateLast = state; // dan plaatsen we nu ook LOW in de laatst gekende state
                                             // Serial.println("I'm here before return 0");
                return 0;                    // en exitten we de functie met LOW
            }
        }
    }

    (*button).stateLast = state; // In dien niet ingedrukt (Paniek, Joystick) saven we de loop gelezen state in de last state

    // return HIGH;
    // Serial.println("I'm here before return 1");
    return 1; // En return HIGH, klaar voor volgende loop.
    // TODO: state check
}

// denderen knop met non-default delay
// misschien kan dit wel mooier met een default delay?
boolean Debounce(struct button_t *button, unsigned long COUNTER_DELAY)
{
    const unsigned long millisRef = millis(); // ophalen huidige tijd

    uint8_t state = digitalRead((*button).pin); // uitlezen PIN (niet-ingedrukt is HIGH, ingedrukt is LOW)
    if (state != (*button).stateLast)           // uitgelezen staat vergelijken met laatst gekende staat - start standaard op HIGH voor Panic knop en Joystick knop
    {
        (*button).lastDebounceTime = millisRef; // loop tijdstip millis opslaan als laatste dendercheck tijdstip
    }

    if ((millisRef - (*button).lastDebounceTime) > COUNTER_DELAY) // loop tijdstip millis verminderen met laatst opgeslaan dendercheck.
    {                                                             // Indien delte groter dan 50ms, verder met de logica.  Indien te klein zou er te veel HIGH,LOW,HIGH zijn
        if (state != (*button).state)                             // als de loop uitgelzen PIN status verschild van de gekende state in de struct
        {
            (*button).state = state; // dan passen we de gekende state aan in de struct, met de nieuwe (loop) state

            if ((*button).state == LOW) // Indien de state LOW (ingedrukt bij Panic en Joystick, omwille van INPUT_PULLUP)
            {
                (*button).stateLast = state; // dan plaatsen we nu ook LOW in de laatst gekende state
                                             // Serial.println("I'm here before return 0");
                return 0;                    // en exitten we de functie met LOW
            }
        }
    }

    (*button).stateLast = state; // In dien niet ingedrukt (Paniek, Joystick) saven we de loop gelezen state in de last state

    // return HIGH;
    // Serial.println("I'm here before return 1");
    return 1; // En return HIGH, klaar voor volgende loop.
    // TODO: state check
}

// Progress Bar https://www.instructables.com/Simple-Progress-Bar-for-Arduino-and-LCD/
// void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn) {
//     double factor = totalCount/80.0;
//     int percent = (count+1)/factor;
//     int number = percent/5;
//     int remainder = percent%5;
//     if(number > 0)
//     {
//        lcd.setCursor(number-1,lineToPrintOn);
//        lcd.write(5);
//     }

//        lcd.setCursor(number,lineToPrintOn);
//        lcd.write(remainder);
//  }
