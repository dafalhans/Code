int keypadRow[4] = {11,10,9,8};
int keypadColumn[4] = {7,6,5,4};
char keypadButtons = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

void setup(){
    Serial.begin(115200);
    for(i=0;i<4;i++){
        pinMode(keypadRow[i], OUTPUT);
        pinMode(keypadColumn[i], INPUT_PULLUP);
        digitalWrite(keypadRow[i], 1);

    }
    

}

void loop(){

    for(int r=0; r<4; r++){

        digitalWrite(keypadRow[r],0);

        for (int k=0;k<4;k++){
            if(digitalRead(kol[k])==0){
                Serial.println("r=" + String(r) + "k=" + String(k) + " toets > " + String(toets[r][k]) + " ingedrukt");
            }
        }

        digitalWrite(rij[r],1);

    }

}