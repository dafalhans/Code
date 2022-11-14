// 5.1) wat wordt er geprint?
// int x=45;
// int y=80;
// while (x<50 and y <100){
//     x=x+1;
//     y=y+1;
//     Serial.print(x); Serial.print(",");Serial.println(y);



void setup() {
    Serial.begin(19200);
    Serial.setTimeout(600000);
}

void loop() {

    int x=45;
    int y=80;
    while (x<50 and y <100){
        x=x+1;
        y=y+1;
    Serial.print(x); Serial.print(",");Serial.println(y);
    }


}
