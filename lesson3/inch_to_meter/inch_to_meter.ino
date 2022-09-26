void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.setTimeout(60000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Testje from VSC");
  float inchesInput;
  float metersOutput;
  Serial.println("Please provide your inches value, to be converted into meters: (press Return key)");
  String inchesInputStr = Serial.readStringUntil('\n');

  //  For the conversion of 1 inch to meter, divide the length value by 39.37.  --> formulate from people smarter then myself
  inchesInput = inchesInputStr.toFloat();

  metersOutput = inchesInput/39.37;

  Serial.println("Your value of " + inchesInputStr + " is calculated into " + metersOutput + " meters");


  //delay(6000);


}