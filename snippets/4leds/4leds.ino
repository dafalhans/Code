int groen = 2;
int rood = 3;
int geel = 4;
int blauw = 5;

int buttonOne = 10;
int buttonTwo =11;
int buttonThree =12;
int buttonFour = 13;


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(600000);
    pinMode(rood,OUTPUT);
    pinMode(buttonOne,INPUT_PULLUP);
    pinMode(groen,OUTPUT);
    pinMode(buttonTwo,INPUT_PULLUP);
    pinMode(geel,OUTPUT);
    pinMode(buttonThree,INPUT_PULLUP);
    pinMode(blauw,OUTPUT);
    pinMode(buttonFour,INPUT_PULLUP);

}

void loop() {

  int sensorValOne = digitalRead(buttonOne);
  int sensorValTwo = digitalRead(buttonTwo);
  int sensorValThree = digitalRead(buttonThree);
  int sensorValFour = digitalRead(buttonFour);
  //print out the value of the pushbutton
  Serial.println(sensorValOne);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:


  if (sensorValOne == HIGH) {
    digitalWrite(rood, LOW);
  } else {
    digitalWrite(rood, HIGH);
  }

  if (sensorValTwo == HIGH) {
    digitalWrite(groen, LOW);
  } else {
    digitalWrite(groen, HIGH);
  }

    if (sensorValThree == HIGH) {
    digitalWrite(geel, LOW);
  } else {
    digitalWrite(geel, HIGH);
  }

    if (sensorValFour == HIGH) {
    digitalWrite(blauw, LOW);
    digitalWrite(geel, LOW);
    digitalWrite(groen, LOW);
  } else {
    digitalWrite(blauw, HIGH);
    digitalWrite(geel, HIGH);
    digitalWrite(groen, HIGH);
  }




}
