
// 5.3) Schrijf een programma dat de gebruiker vraagt om een getal te raden tussen 0-100.
//Er wordt telkens ook uitgeprint te laag, te hoog of correct geraden!
//Zoek in de arduino language reference wat millis() en random() precies doen en gebruik deze in je programma om de random oefeningen te verwezenlijken.


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(60000);
    //randomSeed(millis())
}

void loop() {
    // trick to read in some input from serial console, to produce a semi random number randomSeed;
    Serial.println("TIP: Press F1 in VSC to trigger 'Arduino: Sent Text to Serial Port'");
    //Serial.println("Press Return");
    // Serial.readStringUntil('\n');
    // randomSeed(millis());
    // long randomNumber = random(0,101);

    // Serial.println(String(randomNumber));
    srandom(millis());
    long randomNumber = random(0,101);
    Serial.println(String(randomNumber));
    int counterMax = 0;
    do{
        Serial.println("Guess number between 0 and 100");
        int guess = Serial.readStringUntil('\n').toInt();

        if(guess < randomNumber){
            Serial.println("Too low!");
        }else if(guess > randomNumber){
           Serial.println("Too high!"); 
        }
        if(guess == randomNumber){
            Serial.println("Great success!");
            break;
        }
        counterMax++;
    }while(counterMax < 10);




}
