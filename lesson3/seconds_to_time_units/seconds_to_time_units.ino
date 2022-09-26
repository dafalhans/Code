int secondsInput = 250000;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.setTimeout(60000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Testje from VSC");

    int seconden = secondsInput % 60;
    int gehele_minuten = secondsInput / 60;
    int uren = gehele_minuten / 60;
    int minuten = gehele_minuten % 60;

    Serial.print("Hours: "); Serial.print(uren); Serial.print(" Minutes: "); Serial.print(minuten);Serial.print(" Seconds: "); Serial.print(seconden);
  




  delay(6000);


} 