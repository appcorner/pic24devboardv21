/*
 * File:   EEPROM.c
 * Author: anusorn
 *
 * Created on July 10, 2019, 4:02 PM
 */


#ifndef EEPROM_H
#define	EEPROM_H

void EPWR(unsigned int addr, unsigned char data);
unsigned char EPRD(unsigned int addr);

#endif	//EEPROM_H
