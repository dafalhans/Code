// oef 4.2 ) vraag de gebruiker zijn punten op 100 voor wiskunde, nederlands en geschiedenis. Bereken en geef het
// gemiddelde aan de verbuiker en zeg de gebruiker of hij
// # onvoldoende ( &lt;60) , voldoende ( &gt;=60) , onderscheiding ( &gt;=70) , grote onderscheiding ( &gt;=80) of grootste
// onderscheiding ( &gt;=90) behaalde


// Percentage	GPA
// A	90–100%	4.0
// B	80–89%	3.0
// C	70–79%	2.0
// D	60–69%	1.0
// F	0–59%	0.0


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(600000);
}

void loop() {
    float mathsScore;
    float dutchScore;
    float historyScore;
    char grade;
    float totalPercentage;
    String result;

    Serial.print("Grades for Maths (100): ");
    do{
        mathsScore = Serial.readStringUntil('\n').toFloat();
        Serial.println(String(mathsScore));
    }while(!(mathsScore <= 100 and mathsScore >=0));
    //mathsScore = Serial.readStringUntil('\n').toFloat();
    //Serial.println(String(mathsScore));
    Serial.print("Grades for Dutch (100): ");
    do{
        dutchScore = Serial.readStringUntil('\n').toFloat();
        Serial.println(String(dutchScore));
    }while(!(dutchScore <= 100 and dutchScore >=0));
    Serial.print("Grades for History (100): ");
    do{
        historyScore = Serial.readStringUntil('\n').toFloat();
        Serial.println(String(historyScore));
    }while(!(historyScore <= 100 and historyScore >=0));

    totalPercentage = (mathsScore + dutchScore + historyScore)/3;


    if(totalPercentage<60){
        grade = 'F';
        result = "Onvoldoende";

    }
    else if(totalPercentage<70){
        grade = 'D';
        result = "voldoende";
    }
    else if(grade<80){
        grade = 'C';
        result = "onderscheiding";
    }
    else if(totalPercentage<90){
        grade = 'B';
        result = "grote onderscheiding";
    }
    else{
        grade = 'A';
        result = "grootste onderscheiding";
    }

Serial.println("U heeft een " + result + " ("+ String(grade) + ") behaald.");

}

// String get_string(String ask_string)
// {
//   // ask the question ask_string,
//   // and put some text after it including the stop char
//   // then read a string and return it
//   Serial.print(ask_string);
//   Serial.print("> ");
//   return Serial.readString();
// }
