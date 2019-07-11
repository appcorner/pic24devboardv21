/*
 * File:   pic24devboardv21.h
 * Author: anusorn
 * 
 * pic24 devboard v.2.1 by Mr.Churdpong Lerttamakhun
 *
 * Created on July 1, 2019, 12:20 PM
 */

#ifndef DEVBOARD21_H
#define DEVBOARD21_H

#define uchar       unsigned char
#define uint        unsigned int
#define ulong       unsigned int

#define buzz        LATAbits.LATA4

#define LedG        LATBbits.LATB0
#define LedY        LATBbits.LATB1
#define LedR        LATBbits.LATB2

#define sw1         PORTAbits.RA1
#define sw2         PORTBbits.RB10

void DEVBOARD_Initialize(void);
void DLms(unsigned int x);
void DLus(unsigned int x);

void setup(void);
void loop(void);

#endif //DEVBOARD21_H