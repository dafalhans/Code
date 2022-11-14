

// 5.2) Schrijf een programma dat na het invoeren van een getal, evenveel lijnen met sterretjes ****** print als het getal groot is. 
// Start op de eerste lijn met 1 *, volgende lijn 2 ** enz. 





void setup() {
    Serial.begin(19200);
    Serial.setTimeout(60000);
}

void loop() {
    Serial.println("TIP: Press F1 in VSC to trigger 'Arduino: Sent Text to Serial Port'");
    Serial.println("How many stars and lines do you wanna print? ");
    int starsToPrint = Serial.readStringUntil('\n').toInt();

    int helperNum = 0;

    for(int i=0; i < starsToPrint; i++){

        //Serial.println('*');
        for(int j=0;j<=i;j++){
            Serial.print('*');
        }
        Serial.print('\n');

        // while(helperNum < i){
        //     Serial.print('*');  
        //     helperNum++;
        // }
        // Serial.print('\n');


    }
   

    Serial.println("Play again");

}
