/*
 * File:   pic24devboardv21.c
 * Author: anusorn
 * 
 * pic24 devboard v.2.1 by Mr.Churdpong Lerttamakhun
 *
 * Created on July 1, 2019, 12:20 PM
 */

#include <xc.h>

#include "pic24devboardv21.h"

// Defines for System Clock Timing
//_FOSCSEL(FNOSC_FRCPLL) 
//gives builtin 8mhz, times 4 (pll) = 32MHz, divided by 2 = 16 MHz.
//#define XTFREQ          8000000              // Crystal frequency
//#define PLLMODE         4                    // On-chip PLL setting - must match _FOSC configuration
//#define FCY             (XTFREQ*PLLMODE/2)   // Instruction Cycle Frequency

volatile struct 
{
    unsigned msFlag:1;
} Flag;
         
volatile unsigned int msCnt; // ms counter

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
    TRISBbits.TRISB0=0; // set RB0 output for LedG
    TRISBbits.TRISB1=0; // set RB1 output for LedY
    TRISBbits.TRISB2=0; // set RB2 output for LedR
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
    
    //Timer2 Init
    T2CON=0x0020;  // Internal Clock (FOSC/2), 1:64 prescale 
    PR2=250;  // 0.1mS=0x19 ; 0.5 mS=125 ; 1 mS  0xfa=250
    TMR2=0x0000;  

	_T2IF=0;
	_T2IE=1;  // Enable Timer 2 overflow interrupt

	T2CONbits.TON=1;

    // default
    buzz=1;
    LedG=LedY=LedR=0;
}

void DLms(unsigned int x)
{
    msCnt=x; 
    Flag.msFlag=0;
    while(Flag.msFlag==0) {  }
}

void DLus(unsigned int x)
{
    unsigned int i;
    while(x)
    {
        for(i=3;i>0;i--) {asm("nop");}       
        x--;
    }
}

//-------------------------------------------------------------------------------
// Timer interrupt every 1 ms
//-------------------------------------------------------------------------------
 
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt()
{       
    if(msCnt==0) {Flag.msFlag=1;} else {msCnt--;} // function DLms use
    _T2IF = 0; 
}
