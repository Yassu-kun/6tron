/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"


I2C i2c(I2C1_SDA,I2C1_SCL);

const int BME280_7bit = 0x76;      // 7 bit I2C address
const int BME280_8bit = BME280_7bit << 1; // 8bit I2C address, 0x90

void i2c_write_register(uint8_t reg,uint8_t val){
    char buf[2] = {(char)reg,(char)val};
    i2c.write(BME280_8bit, buf, 2);
}

void i2c_read_regs(uint8_t reg,char *data, int len){
    char r = (char)reg;
    i2c.write(BME280_8bit, &r, 1, true);
    i2c.read(BME280_8bit,data, len);
}

int main()
{   
    static char data[18];
    data[0] = 0x88;
    i2c.write(BME280_8bit, data, 1);
    i2c.read(BME280_8bit, data, 6);
    uint16_t calib_dig_T1 = (data[1] << 8) | data[0];
    uint16_t calib_dig_T2 = (data[3] << 8) | (data[2]);
    uint16_t calib_dig_T3 = (data[5] << 8) | (data[4]);

    
    char node;
    int32_t var1, var2, t_fine;
    char temp_data[3];
    while (1) {
        node= 0xfa;
        i2c.write(BME280_8bit, &node, 1, true);
        i2c.read(BME280_8bit, temp_data, 3, false);


        uint32_t raw_temp = ((temp_data[0] << 12) | (temp_data[1] << 4) | (temp_data[2] >> 4));
        raw_temp &= 0xFFFFF;



         var1 = ((((raw_temp >> 3) - ((int32_t) calib_dig_T1 << 1)))
                    * ((int32_t) calib_dig_T2)) >> 11;

         var2 = (((((raw_temp >> 4) - ((int32_t) calib_dig_T1))
                                    * ((raw_temp >> 4) - ((int32_t) calib_dig_T1))) >> 12)
                    * ((int32_t) calib_dig_T3)) >> 14;

        t_fine = var1 + var2;

        float T = (float)((t_fine * 5 + 128) >> 8) / 100.0;
        
        printf("T = %.2f C\n", T);
        ThisThread::sleep_for(1000ms);
    }
    
}
