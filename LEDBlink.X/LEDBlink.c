/*
 * File:   LEDBlink.c
 * Author: anusorn
 *
 * Created on July 2, 2019, 11:03 AM
 */


#include "pic24devboardv21.h"

void setup(void) {
}

void loop(void) {
    //LedY=1;
    //__delay_ms(500);
    //LedY=0;
    //__delay_ms(500);
    //if (sw1==0) {
    //    while(sw1==0) {__delay_ms(100);}
    //    LedY=0;
    //}
    /*if (sw2==0) {
        while(sw2==0) {__delay_ms(100);}
    }*/
    /*if (sw1 == 0) {
        while(sw1==0){__delay_ms(100);}
        if(LedR==0) LedR=1;
        else LedR=0;
    }*/
    LedR=sw1;
    LedG=buzz=sw2;
}
