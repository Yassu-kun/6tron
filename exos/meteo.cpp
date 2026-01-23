/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "bme280.h"

using namespace sixtron;
using namespace chrono_literals;

I2C i2c(I2C1_SDA, I2C1_SCL);
BME280 sensor(&i2c, BME280::I2CAddress::Address1);

DigitalOut led(LED1);
InterruptIn button(BUTTON1);

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread queue_thread(osPriorityNormal, 4096);

Ticker tick_th;   // Temp
Ticker tick_led;  // LED 

Mutex sensor_mtx;
Mutex print_mtx;

static void task_temp_hum()
{
    bme280_environment_t env;

    sensor_mtx.lock();
    sensor.read_env_data(env);
    sensor_mtx.unlock();

    print_mtx.lock();
    printf("Temp: %.2f C | Hum: %.2f %%\r\n", env.temperature, env.humidity);
    print_mtx.unlock();
}

static void task_pressure()
{
    bme280_environment_t env;

    sensor_mtx.lock();
    sensor.read_env_data(env);
    sensor_mtx.unlock();

    print_mtx.lock();
    printf("Press: %.2f hPa\r\n", env.pressure);
    print_mtx.unlock();
}

static void task_toggle_led()
{
    led = !led;
}

static void on_button_fall()
{
    queue.call(task_pressure);
}



int main()
{   
    sensor.initialize();
    sensor.set_sampling(
        BME280::SensorMode::NORMAL,
        BME280::SensorSampling::OVERSAMPLING_X16,
        BME280::SensorSampling::OVERSAMPLING_X16,
        BME280::SensorSampling::OVERSAMPLING_X16,
        BME280::SensorFilter::OFF,
        BME280::StandbyDuration::MS_500
    );

    queue_thread.start(callback(&queue, &EventQueue::dispatch_forever));

    button.fall(on_button_fall);

    tick_th.attach(callback([] {
        queue.call(task_temp_hum);
    }), 2s);

    tick_led.attach(callback([] {
        queue.call(task_toggle_led);
    }), 5s);

    while (true) {
        ThisThread::sleep_for(1s);
    }
}
