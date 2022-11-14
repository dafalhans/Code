int led = 5;                // the pin that the LED is atteched to
int sensor = 2;             // the pin where the PIR sensor is attached to
int button = 4;             // the pin where the button is attached to
int buzzer = 3;             // the pin where the buzzer is attached to 

bool buttonCurrentState = false;
bool buttonLastState = false;
bool buttonControlState = false;

bool pirCurrentState = false;
bool pirLastState = false;
bool pirControlState = false;

static unsigned long timeStamp = 0;
static bool state_alarm_led = 0;


void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(button,INPUT_PULLUP);
  pinMode(sensor, INPUT);    // initialize sensor as an input
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);        // initialize serial
}

void loop(){

bool loopButtonState = buttonState();
bool loopPirState = pirState();

if(loopButtonState){
    alarmOn();

    if(loopPirState){
        blinkLed(250);
        buzzit();
    }

}



}

bool buttonState(){

    buttonCurrentState = !digitalRead(button);
    if (buttonLastState == false && buttonCurrentState == true){
        if(buttonControlState == false){
        //    digitalWrite(led, LOW);
            buttonControlState = true;
        }
        else{
        //    digitalWrite(led, HIGH);
            buttonControlState = false;
        }
    }
    buttonLastState = buttonCurrentState;

    return buttonControlState;
}

bool pirState(){

    pirCurrentState = !digitalRead(sensor);
    if (pirLastState == false && pirCurrentState == true){
        if(pirControlState == false){
        //    digitalWrite(led, LOW);
            pirControlState = true;
        }
        else{
        //    digitalWrite(led, HIGH);
            pirControlState = false;
        }
    }
    pirLastState = pirCurrentState;

    return pirControlState;
}


void buzzit(){
    debugPrint("buzzit");
    digitalWrite(buzzer, HIGH);
}


void alarmOn(){
    //debugPrint("Alarm Activated");
    blinkLed(500);
}

void alarmOff(){
    debugPrint("Alarm Deactivated");
    pirControlState = 0;
}

// pass blinkSpeed in millis (500 = 0.5 seconds)
void blinkLed(int blinkSpeed)
{
    if (millis() - timeStamp > blinkSpeed){
        timeStamp = millis();
        state_alarm_led = not state_alarm_led; // flip state of led
        digitalWrite(led, state_alarm_led);

    }
}



void debugPrint(String str){
    Serial.println(str);
}

