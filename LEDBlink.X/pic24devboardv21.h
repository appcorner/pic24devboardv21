/*
 * File:   pic24devboardv21.h
 * Author: anusorn
 *
 * Created on July 1, 2019, 12:20 PM
 */


// Defines for System Clock Timing
//_FOSCSEL(FNOSC_FRCPLL) 
//gives builtin 8mhz, times 4 (pll) = 32MHz, divided by 2 = 16 MHz.
#define XTFREQ          8000000              // Crystal frequency
#define PLLMODE         4                    // On-chip PLL setting - must match _FOSC configuration
#define FCY             (XTFREQ*PLLMODE/2)   // Instruction Cycle Frequency

#include "xc.h"
#include <libpic30.h>

#define LedG        LATBbits.LATB0
#define LedY        LATBbits.LATB1
#define LedR        LATBbits.LATB2

#define sw1         PORTAbits.RA1
#define sw2         PORTBbits.RB10

void DEVBOARD_Initialize(void);
void setup(void);
void loop(void);
