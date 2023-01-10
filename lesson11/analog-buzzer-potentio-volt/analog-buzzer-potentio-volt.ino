int ledOut = 8;

void setup() {
    Serial.begin(115200);
    pinMode(ledOut,OUTPUT);
}

void loop() {
    int analogue_input_a5 = analogRead(A5);

    long calculated_volts = analogue_input_a5 / 1023 * 5.0;

    digitalWrite(ledOut,1);
    delay(analogue_input_a5);
    digitalWrite(ledOut,0);
    delay(analogue_input_a5);

    Serial.print("Analoge Input A5 : ");Serial.print(analogue_input_a5);Serial.println(" volt: ");Serial.println(calculated_volts);

}
