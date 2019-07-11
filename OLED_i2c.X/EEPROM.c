/*
 * File:   EEPROM.c
 * Author: anusorn
 *
 * Created on July 10, 2019, 5:50 PM
 */


#include <xc.h>

#include "i2c.h"
#include "pic24devboardv21.h"

void EPWR(unsigned int addr, unsigned char data)
{
if (_I2C_Connect(0xa0)==0)
{
    LedR=1;

    // first word 0x00
    _I2C_SendByte((addr >> 8) & 0x0f);
    //wait for ACK in=0
    if (_I2C_AckTest()!=0) { return; } // addr error

    // secound word 0x00 
    _I2C_SendByte(addr & 0x0f);
    //wait for ACK in=0
    if (_I2C_AckTest()!=0) { return; } // addr error

    // write word data
    _I2C_SendByte(data);
    //wait for ACK in=0
    if (_I2C_AckTest()!=0) { return; } // write error
} else {
    return;  // device not response
}
// stop
_I2C_Stop();

// wait for eprom write data 5ms
DLms(5);

return; // success
}

unsigned char EPRD(unsigned int addr)
{
unsigned char data;
if (_I2C_Connect(0xa0)==0)
{
    LedR=1;
  // first word 0x00
  _I2C_SendByte((addr >> 8) & 0x0f);
  //wait for ACK in=0
  if (_I2C_AckTest()!=0) {  }

  // secound word 0x00 
  _I2C_SendByte(addr & 0x0f);
  //wait for ACK in=0
  if (_I2C_AckTest()!=0) {  }

  _I2C_ReStart();

  _I2C_SendByte(0xa1);
  _I2C_Ack();

  data = _I2C_ReadByte();

  _I2C_Nak();
}
// stop
_I2C_Stop();

return data;
}