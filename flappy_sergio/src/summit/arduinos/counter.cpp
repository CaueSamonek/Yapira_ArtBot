#include <Arduino.h>

#define HALL_PIN 2

int contador = 0;

void setup(){
    pinMode(HALL_PIN, INPUT);
    Serial.begin(9600);
    Serial.println(0);
}

void loop(){
    int hallState = digitalRead(HALL_PIN);
    if (hallState == HIGH){
        contador++;
        Serial.println(contador);
    }

    delay(100);
}
