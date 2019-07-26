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

unsigned char ch;
char DISPLAY[1024];
uint8_t initing;

void setup(void) {

    lcdinit(); //always required in order to put in 4 bit mode 
    lcdcreatecustom();
    lcdbanner();
    
}

void loop(void) {
    LedR=1;
    DLms(500);
    LedR=0;
    DLms(500);
    while(sw1==1) {}
    LedY=1;
    while(sw1==0) {}
    LedY=0;
    lcdcs();
    lcdxy(1,1);
    //lcdcreatecustom();
    //lcdbanner();
}