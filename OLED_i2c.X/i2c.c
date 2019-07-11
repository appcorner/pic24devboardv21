/*
 * File:   i2c.c
 * Author: anusorn
 *
 * Created on July 9, 2019, 12:00 PM
 */

#include <xc.h>

#include "i2c.h"
#include "pic24devboardv21.h"

// I2C
#define I2C_SDA_dir     TRISBbits.TRISB9
#define I2C_SDAi        PORTBbits.RB9
#define I2C_SDAo        LATBbits.LATB9
#define I2C_SCL         LATBbits.LATB11

#define BIT(a,b)  1&(a>>b)


void _I2C_Clk()
{
    I2C_SCL=1;DLus(1);I2C_SCL=0;
}
void _I2C_Start()
{
    I2C_SDA_dir=0;
    I2C_SCL=1;I2C_SDAo=1;DLus(2);
    I2C_SDAo=0;DLus(1);
    I2C_SCL=0;DLus(1);
}
void _I2C_ReStart()
{
    I2C_SDA_dir=0;
    I2C_SDAo=1; DLus(1); I2C_SCL=1;DLus(1);
    I2C_SDAo=0; DLus(1);
    I2C_SCL=0; DLus(1);
}

unsigned char _I2C_AckTest()
{
    unsigned char ack;

    DLus(1);
    I2C_SDAo=1;
    I2C_SDA_dir=1;
    I2C_SDAi=1;
    I2C_SCL=1;DLus(1);
    ack = I2C_SDAi;
    // set to low for next activity
    I2C_SCL=0;DLus(1);

    return ack;
}
void _I2C_Ack()
{
    DLus(1);
    I2C_SDAo=1;
    I2C_SDA_dir=1;
    I2C_SDAi=1;
    _I2C_Clk(); // clock
    DLus(1);
}
void _I2C_Nak()
{
    I2C_SDA_dir=0; // send to device
    I2C_SDAo=1;DLus(1);
    _I2C_Clk(); 
    DLus(1);
}

void _I2C_Stop()
{
    I2C_SDA_dir=0;
    I2C_SCL=0;I2C_SDAo=0;DLus(2);
    I2C_SCL=1;DLus(1);
    I2C_SDAo=1;DLus(1);
}
void _I2C_DelayStop(unsigned int delay)
{
    _I2C_Stop();
    if (delay>0) DLms(delay);
}

void _I2C_SendByte(unsigned char data)
{
    int i;
    I2C_SDA_dir=0;
    for(i=7;i>=0;i--)
    {
        I2C_SDAo=BIT(data,i);
        DLus(1); 
        _I2C_Clk();
        //DLus(1);
    }
}

unsigned char _I2C_ReadByte()
{
    unsigned char data = 0;
    unsigned int i;
    I2C_SDA_dir=1;
    for(i=0;i<8;i++)
    {
        data<<=1;
        I2C_SCL=1;DLus(1);
        data+=I2C_SDAi;
        //DLus(5);
        I2C_SCL=0;DLus(1);
    }

    return data;
}

unsigned char _I2C_Connect(unsigned char address)
{
    _I2C_Start();

    //address frame (id ของ ic)
    //eprom id 0xa0 => 1010 0000
    // bit 7-4 device id
    // bit 3-1 
    // bit 0 r/w 1=>read, 0=>write
    _I2C_SendByte(address);

    return( _I2C_AckTest() );
}
