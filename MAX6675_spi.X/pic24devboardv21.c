/*
 * File:   pic24devboardv21.c
 * Author: anusorn
 *
 * Created on July 1, 2019, 12:20 PM
 */


#include <p24FJ48GA002.h>

#include "pic24devboardv21.h"

void DEVBOARD_Initialize(void)
{
    //CLKDIV=0;
    
    // TRISA: set port direction for RAxx
    // ---- ---- ---4 3210
    // 0000 0000 0000 0000
    
    //TRISA = 0x0002;  // 0000 0000 0000 0010 => 1=input, 0=output 
    TRISAbits.TRISA1=1; // set RA1 input for sw1
    TRISAbits.TRISA4=0; // set RA4 out for buzz

    //ODCA =  0x0000;  // 0000 0000 0000 0000 => 1=Open Drain
    
    // TRISB: set port direction for RBxx  
    // 1111 11
    // 5432 1098 7654 3210
    // 0000 0000 0000 0000
    
    //TRISB = 0x0400; // 0000 0100 0000 0000 => 1=input, 0=output
    TRISBbits.TRISB0=0; // set RB0 out for LedG
    TRISBbits.TRISB1=0; // set RB1 out for LedY
    TRISBbits.TRISB2=0; // set RB2 out for LedR
    TRISBbits.TRISB10=1; // set RB10 input for sw2
    
    //ODCB =  0x0000;  // 0000 0000 0000 0000 => 1=Open Drain
    
    CNPU1bits.CN3PUE=1; // sw1 RA1 pull up
    CNPU2bits.CN16PUE=1; // sw2 RB10 pull up
    
    // AD1PCFG: set analog/digital for ANxx
    // ---1 11
    // ---2 109- --54 3210
    // 0000 0000 0000 0000

    //AD1PCFG = 0x0002; // 0000 0000 0000 0010 => 1=digital, 0=analog
    AD1PCFGbits.PCFG1=1; // set sw1 as digital port
    
    // default
    buzz=1;
    LedG=LedY=LedR=0;
}