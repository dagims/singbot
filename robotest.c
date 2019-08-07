/*
 * File: robocmd.c
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: August, 2019
*/

#define PI_GPIO_ERR
#include "lib/robocmd.c"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]) {
    robo_init();
    robo_cmd(strtol(argv[1], NULL, 10));
    while (1) {
        char cm[3];
        ssize_t n = getline(cm, 3, stdout);
        robo_cmd(strtol(cm, NULL, 10));
        sleep(3);
    }
}
