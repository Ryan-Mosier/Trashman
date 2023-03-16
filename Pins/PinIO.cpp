//
// Created by ryanm on 3/15/2023.
//

#include <iostream>
#include <wiringPi.h>

int main(){
    int x = 0; //input pin #
    wiringPiSetup();
    pinMode(x,INPUT);
    while(true){
        if(digitalRead(x)){
            return 1;
        }
    }
}
