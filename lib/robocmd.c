/*
 * File: robocmd.c
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

#define GPIO_BASE 0x3f200000

#define PI_GPIO_ERR
#include "PI_GPIO.c"
#include <unistd.h>
#include <time.h>

#define IR_OUT (17)


void delay_cmd(long amt)
{
    nanosleep(
        (const struct timespec[]) {{0, amt*833333}},
        NULL);
}

void robo_cmd(int cmd)
{
    GPIO_CLR_N(IR_OUT);
    delay_cmd(8);
    for(int i = 7; i >= 0; i--) 
    {
        GPIO_SET_N(IR_OUT);
        if ((cmd & (1<<i)) == 0)
            delay_cmd(1);
        else
            delay_cmd(4);
        GPIO_CLR_N(IR_OUT);
        delay_cmd(1);
    }
    GPIO_SET_N(IR_OUT);
}

void robo_init()
{
    PI_GPIO_config(IR_OUT, BCM_GPIO_OUT);
    GPIO_SET_N(IR_OUT);
    sleep(3);
}
