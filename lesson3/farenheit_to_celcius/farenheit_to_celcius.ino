void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  Serial.setTimeout(60000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Testje from VSC");
  float farenheitInput;
  float celciusOutput;
  Serial.println("Please provide your Farenheit number, to be converted into degree celcius: (press Return key)");
  String farenheitInputStr = Serial.readStringUntil('\n');

  //  (fahrenheit - 32.0) * 5.0 / 9.0  --> formulate from people smarter then myself
  farenheitInput = farenheitInputStr.toFloat();

  celciusOutput = (farenheitInput - 32) * 5 / 9;

  Serial.println("Your input of " + farenheitInputStr + " degree Farenheit is calculated into " + celciusOutput + " degree Celcius");


  //delay(6000);


}