/*
 * File:   LEDBlink.c
 * Author: anusorn
 *
 * Created on July 2, 2019, 11:03 AM
 */

#include <xc.h>

#include "pic24devboardv21.h"

void setup(void) {
}

void loop(void) {
    LedG=buzz=sw2;
    
    LedY=1; DLms(500);
    LedY=0; DLms(500);
    
    if (sw1 == 0) {
        while(sw1==0){DLms(100);}
        if(LedR==0) LedR=1;
        else LedR=0;
    }
}
