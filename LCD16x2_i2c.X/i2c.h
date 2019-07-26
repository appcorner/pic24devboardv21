/*
 * File:   i2c.h
 * Author: anusorn
 *
 * Created on July 9, 2019, 12:00 PM
 */

#ifndef I2C_H
#define	I2C_H

void _I2C_Initialize(void);

void _I2C_Clk();
void _I2C_Start();
void _I2C_ReStart();
unsigned char _I2C_AckTest();
void _I2C_Ack();
void _I2C_Nak();
void _I2C_Stop();
void _I2C_DelayStop(unsigned int delay);
void _I2C_SendByte(unsigned char data);
unsigned char _I2C_ReadByte();
unsigned char _I2C_Connect(unsigned char address);

#endif	//I2C_H