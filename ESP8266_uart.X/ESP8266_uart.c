/*
 * File:   ESP8266_uart.c
 * Author: anusorn
 *
 * Created on July 12, 2019, 5:07 PM
 */


#include <xc.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "pic24devboardv21.h"
#include "i2c.h"
#include "OLED.h"

#define SYSCLK          16000000
#define BAUDRATE        115200
#define ESP8266_en      LATBbits.LATB10  // out

unsigned int i,j;
char DISPLAY[1024];

#define maxINB1         1024 // max input buffer (bytes)
volatile unsigned int Rx1Cnt;
unsigned char INB[maxINB1+1]; // message before LF and CR

void PutChrU1(unsigned char c)
{
    while(U1STAbits.UTXBF) {}
    U1TXREG=c;
}
void PutStrU1(char *data)
{  
    while(*data)
    {
        PutChrU1(*data);
        data++;
    }
}

void setup(void) {
    _I2C_Initialize();

    TRISBbits.TRISB10=0; // set RB10 output for EN => ESP8299 EN
    TRISBbits.TRISB12=1; // set RB12 input for RX
    TRISBbits.TRISB13=0; // set RB13 output for TX
    
    AD1PCFGbits.PCFG11=1; // set RX (RB12) as digital port
    AD1PCFGbits.PCFG12=1; // set TX (RB13) as digital port

    //------------- 
    // InitU1
    U1MODE=0; // clear mode register
    U1MODEbits.UARTEN=1; // enable UART (enable UART)
    U1MODEbits.BRGH=1; // set high speed
    U1BRG=(unsigned int)((((double)SYSCLK/2.0) / ((double)BAUDRATE*4.0))-1.0);

    U1STA=0; // clear status register
    U1STAbits.UTXEN=1; // enable UART1 transmitter (receiver is always enabled)
    //U1STAbits.UTXISEL0=0; // enable TX buffer empty interrupt
    //U1STAbits.UTXISEL1=1; // enable TX buffer empty interrupt
    //U1STAbits.URXISEL=0; // enable RX data available interrupt

    //IEC0bits.U1TXIE=1; // enable UART1 TX interrupts
    IEC0bits.U1RXIE=1; // enable UART1 RX interrupts
    
    // Assign U1RX to Pin RP12 (RB12)
    RPINR18bits.U1RXR=12;
    // Assign U1TX to Pin RP13 (RB13) 
    RPOR6bits.RP13R=3;

    IFS0bits.U1RXIF=0; // Clear RX interrupt flag
    
    ESP8266_en=1;

    //---------------------------
    // ssd1306 128x64
    //---------------------------
    ssd1306Init();
    //--- clear screen 
    OLedClr();

    DLms(5000);
    Rx1Cnt=0; INB[Rx1Cnt]=0;
    PutStrU1("AT+RST\r\n");
    DLms(1000);
}

void loop(void) {
    LedR=1;
    while(sw1==1) {}
    //DLms(1000);
    //OLedClr();
    LedG=0;
    LedY=0;
    LedR=0;
    Rx1Cnt=0; INB[Rx1Cnt]=0;
    PutStrU1("AT\r\n");

    while(sw1==0) {}
    DLms(2000);
    LedWStr13TH(1, INB);
    Rx1Cnt=0; INB[Rx1Cnt]=0;
}

//-------------------------------------------------------------------------------
// Rx1 Interrupt
void __attribute__((interrupt, auto_psv)) _ISR _U1RXInterrupt()
{
    INB[Rx1Cnt]=U1RXREG&0xff;        
    if (INB[Rx1Cnt-1]=='O'&&INB[Rx1Cnt]=='K') {
        LedG=1;
    }
    LedY=1;
    //------------------------------------------
    //if (INB[Rx1Cnt]!='\r' && INB[Rx1Cnt]!='\n') {
        Rx1Cnt++;
    //}
    if (Rx1Cnt>=maxINB1-1) { Rx1Cnt=0; }
    INB[Rx1Cnt]=0;
    //----------------------------------------
    IFS0bits.U1RXIF=0; // clear interrupt flag       
}
