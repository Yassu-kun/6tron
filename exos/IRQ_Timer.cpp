/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

namespace {
#define PERIOD_MS 200ms
}
using namespace std::chrono;

const std::chrono::microseconds periods[] = {1s, 750ms, 500ms, 250ms, 100ms};

Timer t;
Ticker blink;
int c = 0; //flag
int c2 = 0; //flag 2
//DigitalIn  button(BUTTON1); //Pour le poling
InterruptIn button(BUTTON1); // Pour l'IRQ
DigitalOut myled(LED1);



void flip(){
    myled =!myled;
}

void press(){
    c2 = 1;
}

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
    //button.rise(&press);
    //int i = 0;
    //blink.attach(&flip, periods[i]);

    button.fall(&low);
    
    if (myled.is_connected()){
        printf("led initialized and connected!\n\r");
    }
    
    while (true) {

        /*if (c2){
            c2 = 0;
            i = (i + 1) % 5;
            blink.detach();
            blink.attach(&flip, periods[i]);

        }*/
        
        //printf("Groupe 2 : salut! \n");
        //ThisThread::sleep_for(PERIOD_MS / 2);
        //myled = !myled;     // chaque boucle pour le clignotement      
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
