/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "bme280.h"

using namespace sixtron;





int main()
{   
    I2C i2c(I2C1_SDA,I2C1_SCL);
    BME280 sensor(&i2c,BME280::I2CAddress::Address1);
    
    sensor.initialize();

    sensor.set_sampling(
            BME280::SensorMode::NORMAL,
            BME280::SensorSampling::OVERSAMPLING_X16,
            BME280::SensorSampling::OVERSAMPLING_X16,
            BME280::SensorSampling::OVERSAMPLING_X16,
            BME280::SensorFilter::OFF,
            BME280::StandbyDuration::MS_500
        );
     
    
    while (true) {
        bme280_environment_t c;
        sensor.read_env_data(c);
        printf("Temp: %.2f C \n Hum: %.2f %% \n Press: %.2f hPa\r\n",
               c.temperature,
               c.humidity,
               c.pressure );
        printf("\n");  
        ThisThread::sleep_for(3s);
    }
}
