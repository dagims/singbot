/*
 * File: robo-guile.c
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

#define GPIO_BASE 0x3f200000
#define PI_GPIO_ERR
#include <libguile.h>
#include <unistd.h>
#include <time.h>
#include "lib/PI_GPIO.c"

#define IR_OUT (17)

void delay_cmd (long amt)
{
    nanosleep(
        (const struct timespec[]) {{0, amt*833333}},
        NULL);
}

SCM scm_cmd_robot(SCM cmd_scm)
{
    int cmd = scm_to_int(cmd_scm);
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
    return scm_from_int(0);
}

SCM scm_init_robot()
{
    PI_GPIO_config(IR_OUT, BCM_GPIO_OUT);
    GPIO_SET_N(IR_OUT);
    return scm_from_int(0);
}

void scm_init_binding()
{
    scm_c_define_gsubr("robot-cmd", 1, 0, 0, scm_cmd_robot);
    scm_c_define_gsubr("robot-init", 0, 0, 0, scm_init_robot);
}


