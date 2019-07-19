/*
 * File:   OLED.c
 * Author: anusorn
 *
 * Created on July 10, 2019, 4:02 PM
 */


#include <xc.h>

#include "i2c.h"
#include "tahoma.h"

#define CH_WIDTH_13 13 // char width

volatile unsigned int _colBuf;
volatile unsigned int _chCnt;
volatile unsigned char _chBuf13[CH_WIDTH_13*2];

// 0x78 , 0x00 , cmd
void OLEDwrCmd(unsigned char cmd)
{
    //DLus(2);
    _NSTDIS=1;
        
    _I2C_Connect(0x78);
    //if(_I2C_Connect(0x78))
    //{
        // write 0x00 (command index)
        _I2C_SendByte(0x00);
        _I2C_Ack();

        // ------------send data ---------------
        _I2C_SendByte(cmd);
        _I2C_Ack();
    //}
    // stop
    _I2C_Stop();
    
    _NSTDIS=0;
}
//--------------------------------------


//--------------------------------------
// 
void i2c_start(void)
{
    _NSTDIS=1;
    _I2C_Start();
}
void i2c_write(unsigned char dat)
{ 
    _I2C_SendByte(dat);
    _I2C_Ack();
}
void i2c_stop(void)
{
    _I2C_Stop();
    _NSTDIS=0; 
}

void ssd1306Init(void)
{    
    OLEDwrCmd(0xAE); // Set display OFF		

    OLEDwrCmd(0xD4); // Set Display Clock Divide Ratio / OSC Frequency
    OLEDwrCmd(127);  // Display Clock Divide Ratio / OSC Frequency 

    OLEDwrCmd(0xA8); // Set Multiplex Ratio
    OLEDwrCmd(63);   // Multiplex Ratio for 128x64 (64-1)

    OLEDwrCmd(0xD3); // Set Display Offset
    OLEDwrCmd(0x00); // Display Offset

    OLEDwrCmd(0x40); // Set Display Start Line
    OLEDwrCmd(0x00); // Set Display Start Line == add from init3


    OLEDwrCmd(0x8D); // Set Charge Pump
    OLEDwrCmd(0x14); // Charge Pump (0x10 External, 0x14 Internal DC/DC)

    OLEDwrCmd(0xA1); // Set Segment Re-Map
    OLEDwrCmd(0xC8); // Set Com Output Scan Direction 0xc8=200

    OLEDwrCmd(0xD5); // Set clock div == add from init3
    OLEDwrCmd(128);  // 

    OLEDwrCmd(0x20); // Set mem add mode == add from init3  ######### important
    OLEDwrCmd(0x00); // 


    OLEDwrCmd(0xDA); // Set COM Hardware Configuration
    OLEDwrCmd(0x12); // COM Hardware Configuration

    OLEDwrCmd(0x81); // Set Contrast
    OLEDwrCmd(0xff); // Contrast

    OLEDwrCmd(0xD9); // Set Pre-Charge Period
    OLEDwrCmd(0xF1); // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

    OLEDwrCmd(0xDB); // Set VCOMH Deselect Level
    OLEDwrCmd(0x40); // VCOMH Deselect Level

    OLEDwrCmd(0xA4); // Set all pixels OFF
    OLEDwrCmd(0xA6); // Set display not inverted
    //OLEDwrCmd(0xA7); // Set display inverted
    OLEDwrCmd(0xAF); // Set display On
}
//--- clear screen ------------------
void OLedClr(void)
{
    unsigned int i;

    // set Column start and end 128 bits
    OLEDwrCmd(0x21); //set column
    OLEDwrCmd(0x00); //start
    OLEDwrCmd(127);  //end
    // set page(Row) start and end 8 rows (64bits)
    OLEDwrCmd(0x22); //set page
    OLEDwrCmd(0x00); //start
    OLEDwrCmd(7);    //end
    //write data 0 to clear screen ---
    i2c_start();
    i2c_write(0x78); // begin of cmd/data
    i2c_write(0x40); // data write
    //clear
    for(i=0;i<1024;i++) {i2c_write(0);}
    i2c_stop();
}

void LedWI13(unsigned char row, unsigned char col)
{
    unsigned char i;
    //row 0-1,2-3,4-5,6-7
    switch(row)
    {
        case 1: OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(1);break;
        case 2: OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(3);break;
        case 3: OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(5);break;
        default: OLEDwrCmd(0x22); OLEDwrCmd(6); OLEDwrCmd(7);break;
    }
    _chCnt=0;
    _colBuf=col*CH_WIDTH_13;
    for(i=0;i<CH_WIDTH_13*2;i++) {_chBuf13[i] = 0x00;}
}

int ThaiLevel(unsigned char ch)
{
	if (ch>=216 && ch<=218) return -1;
	else if ((ch==209) || (ch>=212 && ch<=215)) return 1;
	else if (ch>=231 && ch<=238) return 2;
	else return 0;
}

void LedWD13TH(unsigned char dat)
{
    unsigned char i,j;
    unsigned int c;

    if (ThaiLevel(dat)==0 && _chCnt>0)
    {
        _colBuf+=CH_WIDTH_13; //next start column 
        _chCnt++;
    } else if (_chCnt==0) {
        _chCnt++;
    }
    OLEDwrCmd(0x21); OLEDwrCmd(_colBuf); OLEDwrCmd(_colBuf+CH_WIDTH_13-1);
    i2c_start();
    i2c_write(0x78); //start instruction
    i2c_write(0x40); //write data
    //
    c=(dat-32)*CH_WIDTH_13*2;
    if (ThaiLevel(dat)==0)
    {
        // copy font to buf
        for(i=0;i<CH_WIDTH_13*2;i++)
        {
            _chBuf13[i] = text13n[c+i];
        }
    } else {
        // merge font to buf
        for(j=0;j<2;j++)
        {
            for(i=0;i<CH_WIDTH_13;i++)
            { 
                if (j==0) _chBuf13[j+(i*2)] = (0xe0 & text13n[c+j+(i*2)]) | _chBuf13[j+(i*2)];
                else _chBuf13[j+(i*2)] = (0x0f & text13n[c+j+(i*2)]) | _chBuf13[j+(i*2)];
            }
        }
    }
    for(i=0;i<CH_WIDTH_13;i++)
    { 
        i2c_write(_chBuf13[(i*2)+1]);
    }
    for(i=0;i<CH_WIDTH_13;i++)
    { 
        i2c_write(_chBuf13[(i*2)]);
    }
    i2c_stop(); 
}

void LedWStr13TH(unsigned char row, unsigned char *dat)
{
    LedWI13(row, 0);
    while(*dat)
    {
        if(*dat=='\r') {
            LedWI13(row, 0);
        }
        else if(*dat=='\n'){
            row++;
            if(row>4)row=1;
            LedWI13(row, 0);
        }
        else {
            LedWD13TH(*dat);
        }
        dat++;
    }
}
