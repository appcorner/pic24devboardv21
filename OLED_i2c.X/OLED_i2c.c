/*
 * File:   OLED_i2c.c
 * Author: anusorn
 *
 * Created on July 9, 2019, 2:32 PM
 */

#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pic24devboardv21.h"
#include "OLED.h"
#include "EEPROM.h"

unsigned char ch;
char DISPLAY[1024];

void setup(void) {
    
    //----------------------------------------------------------------
    // ssd1306 128x64
    //---------------------------
    ssd1306Init();
    //--- clear screen 
    OLedClr();

    LedWStr13TH(1,"ิอยู่นี่นะ");
    LedWStr13TH(2,"อยู่นี่นะ");
    //LedWStr13TH(2,"พระนครอัน");
    //LedWStr13TH(3,"กว้างใหญ่");

    LedWD13TH('*');
    
    LedWI13(3, 0);
    LedWD13TH('1');
    LedWD13TH('2');
    LedWD13TH('3');
    LedWD13TH('4');
    LedWD13TH('5');
    LedWD13TH('6');
    LedWD13TH('7');
    LedWD13TH('8');
    LedWD13TH('9');
    LedWD13TH('A');
    //LedWD13TH('ก');
    
    ch='ข';
    EPWR(0x0002, ch);
    ch=EPRD(0x0002);
    if(ch==0xa2) { LedY=1; }
    sprintf(DISPLAY,"ch=0x%x",ch);
    LedWStr13TH(4,DISPLAY);
    
}

void loop(void) {
    LedR=1;
    //EPWR(0x00, 'B');
    DLms(500);
    LedR=0;
    DLms(500);
    //while(sw1==1) {}
}