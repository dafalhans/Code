void alarmOn(){
    debugPrint("Alarm Activated");
}

void alarmOff(){
    debugPrint("Alarm Deactivated");
}



void debugPrint(String str){
    Serial.println(str);
}