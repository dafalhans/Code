// oef 4.1 ) vraag de gebruiker 2 getallen en zeg hem daarna of het eerste of tweede getal het grootste was of zeg hem dat
// ze gelijk waren.

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(600000);

    
}

void loop() {
    float firstEntry;
    float secondEntry;
    Serial.print("Please provide your first number: ");
    firstEntry = Serial.readStringUntil('\n').toFloat();
    Serial.println(String(firstEntry));
    Serial.print("Please provide your second number: ");
    secondEntry = Serial.readStringUntil('\n').toFloat();
    Serial.println(String(secondEntry));

    if(secondEntry>firstEntry){
        Serial.println(String(secondEntry) + " is larger than " + String(firstEntry));
    }
    else if(secondEntry<firstEntry){
        Serial.println(String(secondEntry) + " is smaller than " + String(firstEntry));
    }
    else if(secondEntry=firstEntry){
        Serial.println(String(secondEntry) + " is equal to " + String(firstEntry));
    }
    else{
        Serial.println("Something's wrong...");
    }   

}
