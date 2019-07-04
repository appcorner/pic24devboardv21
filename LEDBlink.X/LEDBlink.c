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
    LedG=buzz=sw2;
    
    //LedY=1; __delay_ms(500);
    //LedY=0; __delay_ms(500);
    
    if (sw1 == 0) {
        while(sw1==0){__delay_ms(100);}
        if(LedR==0) LedR=1;
        else LedR=0;
    }
}
