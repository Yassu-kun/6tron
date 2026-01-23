/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

Mutex mtx;

void ping() {
    for (int i = 0; i < 100; i++) {
        mtx.lock();
        printf("Ping\n");
        mtx.unlock();
    }
}

void pong() {
    for (int i = 0; i < 100; i++) {
        mtx.lock();
        printf("Pong\n");
        mtx.unlock();
    }
}

Thread th_ping;
Thread th_pong;


int main()
{   
    DigitalOut myled(LED1);
    th_ping.start(ping);
    th_pong.start(pong);

    osThreadSetPriority(osThreadGetId(), osPriorityLow);

    while (true) {
        myled = !myled;
        mtx.lock();
        printf("Alive!\n");
        mtx.unlock();
        ThisThread::sleep_for(500ms);
    }
}
