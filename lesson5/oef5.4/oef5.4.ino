
//5.4) Schrijf een programma dat de gebruiker vraagt om een getal te raden tussen 0-100.
//Er wordt telkens ook uitgeprint te laag , te hoog of correct geraden!
//Indien de gebruiker het getal raad op minder dan 100 sec dan flikkert een groene led 10 keer 0.5 sec aan/uit, 
//indien het getal niet geraden werd binnen 100 sec dan flikkert een rode led 10 keer 0.5 sec aan/uit!


int groen = 2;
int rood = 3;
int geel = 4;
int blauw = 5;

// int buttonOne = 10;
// int buttonTwo =11;
// int buttonThree =12;
// int buttonFour = 13;


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(6000000);
    pinMode(rood,OUTPUT);
    pinMode(groen,OUTPUT);
    // pinMode(buttonOne,INPUT_PULLUP);
    // pinMode(groen,OUTPUT);
    // pinMode(buttonTwo,INPUT_PULLUP);
    pinMode(geel,OUTPUT);
    // pinMode(buttonThree,INPUT_PULLUP);
    pinMode(blauw,OUTPUT);
    // pinMode(buttonFour,INPUT_PULLUP);

}

void loop() {

    Serial.println("TIP: Press F1 in VSC to trigger 'Arduino: Sent Text to Serial Port'");
    //Serial.println("Press Return");
    // Serial.readStringUntil('\n');
    // randomSeed(millis());
    // long randomNumber = random(0,101);

    // Serial.println(String(randomNumber));

    int startTheTime = millis() * 1000;
    int endTime = 100000;
    randomSeed(millis());
    long randomNumber = random(0,101);
    Serial.println(String(randomNumber));
    Serial.println(String(startTheTime));

    digitalWrite(rood, HIGH);
    delay(2000);
    digitalWrite(rood, LOW);
    digitalWrite(geel, HIGH);
    delay(2000);
    digitalWrite(geel, LOW);
    digitalWrite(groen, HIGH);
    delay(2000);
    digitalWrite(groen, LOW);
    digitalWrite(blauw, HIGH);
    delay(2000);
    digitalWrite(blauw, LOW);


  //  do{

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
                if(millis() - startTheTime<=100000){
                    int starttime = millis();
                    int endtime = starttime;
                    while ((endtime - starttime) <=10000) // do this loop for up to 1000mS
                    {
                        digitalWrite(groen, HIGH);
                        delay(500);
                        digitalWrite(groen, LOW);
                        delay(500);  
                    //loopcount = loopcount+1;
                    endtime = millis();
                    }
                        // digitalWrite(groen, HIGH);
                        // delay(500);
                        // digitalWrite(groen, LOW);
                        // delay(500);  
                }
                else{
                    int starttime = millis();
                    int endtime = starttime;
                    while ((endtime - starttime) <=10000) // do this loop for up to 1000mS
                    {
                        digitalWrite(rood, HIGH);
                        delay(500);
                        digitalWrite(rood, LOW);
                        delay(500); 
                        endtime = millis();
                    }
                }
                Serial.println("Great success!");
                break;
            }

            counterMax++;

            Serial.println(String(counterMax));
            //Serial.println(String(startTheTime))
            if(millis() - startTheTime>10000){
                int starttime = millis();
                int endtime = starttime;
                while ((endtime - starttime) <=10000) // do this loop for up to 1000mS
                {
                    digitalWrite(rood, HIGH);
                    delay(500);
                    digitalWrite(rood, LOW);
                    delay(500); 
                    endtime = millis();
                }
            }

        }while(counterMax < 10);
    //    }while((counterMax < 10) && ((millis() * 1000)<=100000));

    //}while((millis() * 1000)<=100000)




//     int counterMax = 0;
//     do{
//         Serial.println("Guess number between 0 and 100");
//         int guess = Serial.readStringUntil('\n').toInt();

//         if(guess < randomNumber){
//             Serial.println("Too low!");
//         }else if(guess > randomNumber){
//            Serial.println("Too high!"); 
//         }
//         if(guess == randomNumber){
//             Serial.println("Great success!");
//             break;
//         }
//         counterMax++;
//     }while(counterMax < 10);


//   if (sensorValOne == HIGH) {
//     digitalWrite(rood, LOW);
//   } else {
//     digitalWrite(rood, HIGH);
//   }

//   if (sensorValTwo == HIGH) {
//     digitalWrite(groen, LOW);
//   } else {
//     digitalWrite(groen, HIGH);
//   }

//     if (sensorValThree == HIGH) {
//     digitalWrite(geel, LOW);
//   } else {
//     digitalWrite(geel, HIGH);
//   }

//     if (sensorValFour == HIGH) {
//     digitalWrite(blauw, LOW);
//     digitalWrite(geel, LOW);
//     digitalWrite(groen, LOW);
//   } else {
//     digitalWrite(blauw, HIGH);
//     digitalWrite(geel, HIGH);
//     digitalWrite(groen, HIGH);
//   }




}
