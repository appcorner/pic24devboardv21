/*
 * File:   main.c
 * Author: anusorn
 *
 * Created on July 1, 2019, 12:20 PM
 */

#include <xc.h>

#include "pic24devboardv21.h"

//_CONFIG1(JTAGEN_OFF & GCP_ON & COE_OFF & FWDTEN_OFF & FWPSA_PR32 & WDTPS_PS1024 )
//_CONFIG2(FNOSC_FRCPLL & POSCMOD_NONE & FCKSM_CSDCMD & OSCIOFNC_OFF ) => no crystal
//_CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & FCKSM_CSDCMD & OSCIOFNC_OFF ) => external crystal
int CONFIG2 __attribute__((space(prog), address(0x83FC))) = 0xF9BF ;
//_CONFIG2(
//    POSCMOD_NONE &       // Primary Oscillator Select (Primary oscillator disabled)
//    I2C1SEL_PRI &        // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
//    IOL1WAY_ON &         // IOLOCK Protection (Once IOLOCK is set, cannot be changed)
//    OSCIOFNC_OFF &       // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
//    FCKSM_CSDCMD &       // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
//    FNOSC_FRCPLL &       // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
//    SOSCSEL_SOSC &       // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
//    WUTSEL_LEG &         // Wake-up timer Select (Legacy Wake-up Timer)
//    IESO_ON              // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)
//);
int CONFIG1 __attribute__((space(prog), address(0x83FE))) = 0x1F6A ;
//_CONFIG1(
//    WDTPS_PS1024 &       // Watchdog Timer Postscaler (1:1,024)
//    FWPSA_PR32 &         // WDT Prescaler (Prescaler ratio of 1:32)
//    WINDIS_ON &          // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
//    FWDTEN_OFF &         // Watchdog Timer Enable (Watchdog Timer is disabled)
//    ICS_PGx1 &           // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
//    GWRP_OFF &           // General Code Segment Write Protect (Writes to program memory are allowed)
//    GCP_ON &             // General Code Segment Code Protect (Code protection is enabled for the entire program memory space)
//    JTAGEN_OFF           // JTAG Port Enable (JTAG port is disabled)
//);

/*
                         Main application
 */
int main(void)
{
    DEVBOARD_Initialize();

    setup();
    
    while (1)
    {
        loop();  
    }

    return 1;
}
/**
 End of File
*/

