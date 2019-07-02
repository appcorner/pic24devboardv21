/*
 * File:   LEDBlink.c
 * Author: anusorn
 *
 * Created on July 2, 2019, 11:03 AM
 */


#include "pic24devboardv21.h"

void setup(void) {
}

void loop(void) {
    LedR=1;
    LedY=1;
    LedG=1;
    __delay_ms(1000);
    LedR=0;
    LedY=0;
    LedG=0;
    __delay_ms(1000);
}
