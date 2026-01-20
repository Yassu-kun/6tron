/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

namespace {
#define PERIOD_MS 200ms
}
using namespace std::chrono;

Timer t;
int c = 0; //flag
//DigitalIn  button(BUTTON1);
InterruptIn button(BUTTON1);
DigitalOut myled(LED1);

void high(){  
    myled = 1;
    t.reset();
    t.start();
}


void low(){
    myled = 0;
    t.stop();
    c = 1;
}

int main()
{   
    button.mode(PullDown);
    button.rise(&high);
    button.fall(&low);
    
    if (myled.is_connected()){
        printf("led initialized and connected!\n\r");
    }
    
    while (true) {
        //printf("Groupe 2 : salut! \n");
        //ThisThread::sleep_for(PERIOD_MS / 2);
        //myled = !myled;     // chaque boucle      
        //myled = button;  // poling
        /*ThisThread::sleep_for(PERIOD_MS / 4);
        myled.write(0);*/     
        if(c){
            c=0;
            printf("The time taken was %llu microseconds\n", duration_cast<microseconds>(t.elapsed_time()).count());
        }
        ThisThread::sleep_for(PERIOD_MS / 4);
    }
}
