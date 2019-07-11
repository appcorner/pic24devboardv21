/*
 * File:   MAX6675_spi.c
 * Author: anusorn
 *
 * Created on July 4, 2019, 3:05 PM
 */

#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pic24devboardv21.h"
#include "i2c.h"
#include "OLED.h"

#define MAX6675_cs      LATBbits.LATB10  // out
#define MAX6675_so      PORTBbits.RB12   // in
#define MAX6675_sck     LATBbits.LATB13  // out

unsigned int i,j;
char DISPLAY[1024];

void setup(void) {
    _I2C_Initialize();

    TRISBbits.TRISB10=0; // set RB10 output for CS => SPI CS
    TRISBbits.TRISB12=1; // set RB12 input for SO => SPI SO
    TRISBbits.TRISB13=0; // set RB13 output for SCK => SPI SCK
    
    AD1PCFGbits.PCFG11=1; // set SO (RB12) as digital port
    AD1PCFGbits.PCFG12=1; // set SCK (RB13) as digital port

    //---------------------------
    // ssd1306 128x64
    //---------------------------
    ssd1306Init();
    //--- clear screen 
    OLedClr();

    LedWStr13TH(1, "Temp = ");        
}

void loop(void) {
    LedR=1;
    while(sw1==1) {}
    LedR=0;
    
    MAX6675_cs=1; MAX6675_sck=0; MAX6675_so=1; DLms(300);
    MAX6675_cs=0; DLms(2);
    j=0;
    for(i=0;i<16;i++){
        j<<=1;
        MAX6675_sck=1; DLms(1);
        if(MAX6675_so){j+=1;LedG=1;}
        MAX6675_sck=0; DLms(1);
    }

    //if(j&0x04){Flag.temp1OCF=1;}
    j>>=3;

    i=(unsigned long)(j*25);
    //float tmp = i/100.0;
    sprintf(DISPLAY,"j=0x%x ",  j);
    LedWStr13TH(4, DISPLAY);
    
    sprintf(DISPLAY,"i=%d ",  i);
    LedWStr13TH(3, DISPLAY);

    sprintf(DISPLAY,"%d.%02d ",  i/100,  i%100);
    LedWStr13TH(2, DISPLAY);

    while(sw1==0) {}
}
