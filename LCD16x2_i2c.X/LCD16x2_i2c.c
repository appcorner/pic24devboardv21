/*
 * File:   LCD16x2_i2c.c
 * Author: anusorn
 *
 * Created on July 22, 2019, 8:07 AM
 */


#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pic24devboardv21.h"
#include "LCD.h"

char DISPLAY[1024];
int counter;

void setup(void) {

    lcdinit();
    lcdbanner();
    counter = 0;
    sprintf(DISPLAY, "count = %d ", counter);
    lcdxy(1,2);
    lcdtext(DISPLAY);
    
}

void loop(void) {
    if(sw1==0) {
        LedG=1;
        while(sw1==0) {DLms(100);}
        counter++;
        LedG=0;
    }
    if(sw2==0) {
        LedY=1;
        while(sw2==0) {DLms(100);}
        counter--;
        LedY=0;
    }
    sprintf(DISPLAY, "count = %d ", counter);
    lcdxy(1,2);
    lcdtext(DISPLAY);
}