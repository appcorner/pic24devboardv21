/*
 * File:   LCD.c
 * Author: anusorn
 *
 * Created on July 22, 2019, 8:09 AM
 * 
 * ref: https://www.microchip.com/forums/m907408.aspx
 * 
 */


#include <xc.h>
#include <stdint.h>
#include <string.h>

#include "pic24devboardv21.h"
#include "i2c.h"
#include "LCD.h"

#define LCD_Addr 0x27 //0x00100111 

// for Hitachi HD44780U chip
#define LINE_1_ADDRESS 0x00
#define LINE_2_ADDRESS 0x40 
#define LINE_3_ADDRESS 0x14
#define LINE_4_ADDRESS 0x54 

//I2C1_MESSAGE_STATUS status;
int lcd_j,lcd_max,lcd_tempbyte;
int lcd_cd_state; // HD44780U LCD Instruction Type: COMMAND=0, DATA=1
uint8_t sbs[256]; //max length of an i2c send in bytes is 256 could reduce this to 120 (120 is max needed so far 20 LCD characters at once, 6 per character)

void I2C_Write(uint8_t num)
{
    //DLus(2);
    _NSTDIS=1;
        
    _I2C_Connect(LCD_Addr<<1);
    //if(_I2C_Connect(0x78))
    //{
        // write 0x00 (command index)
        //_I2C_SendByte(0x00);
        //_I2C_Ack();

        // ------------send data ---------------
    int i;
    for(i=0;i<num;i++) {
        _I2C_SendByte(sbs[i]);
        _I2C_Ack();
    }
    //}
    // stop
    _I2C_Stop();
    
    _NSTDIS=0;
}

void i2c_write_sbs(uint8_t num) {
    I2C_Write(num);
    //I2C1_MasterWrite(&sbs,num,LCD_Addr,&status); //this sends i2c START sequence, LCD_Addr<<1, sbs bytes and i2c STOP sequence
    //long zz=0;
    //while (status){
    //    zz++; 
    //    if (zz > 200000) break;
    //};
    DLus(1500); //adjust if necessary, not consistent at 300us (get garbled or mispositioned text if too low))
    //appears to need about 1500us total between sequential i2c streams (start,address,data,stop)
}

void LCD_Send_Byte(uint8_t dc, uint8_t n){ //dc = 1 is data 9-D-9, dc = 0 is command 8-C-8
   if(dc){
      lcd_cd_state=1; // data
   } else {
      lcd_cd_state=0; // command
   }
   // Send upper nibble
   lcd_tempbyte=(n&0xF0)|lcd_cd_state; // Write Most Significant Nibble of text[j] (LCD in 4-bit mode)
   sbs[0]=(lcd_tempbyte|0x08); // Least Significant Nibbles for I2C DATA = 9-D-9 (LSB=1)
   sbs[1]=(lcd_tempbyte|0x0C);
   sbs[2]=(lcd_tempbyte|0x08);
   // Send lower nibble
   lcd_tempbyte=(n<<4)|lcd_cd_state; // Write Most Significant Nibble of text[j] (LCD in 4-bit mode)
   sbs[3]=(lcd_tempbyte|0x08); // Least Significant Nibbles for I2C DATA = 9-D-9 (LSB=1)
   sbs[4]=(lcd_tempbyte|0x0C);
   sbs[5]=(lcd_tempbyte|0x08);
   i2c_write_sbs(6);
}

void lcdcs(){
    LCD_Send_Byte(0,1); // Clear screen 
}

void lcdcmd(uint8_t c){
    LCD_Send_Byte(0,c);
}

void lcddata(uint8_t c){
    LCD_Send_Byte(1,c);
}

void lcdtext(const char text[]) {
    //function to write a string to LCD via I2C
    lcd_max=strlen(text);
    uint8_t nin=6*lcd_max;
    for(lcd_j=0; lcd_j<lcd_max; ++lcd_j) {
        // Send upper nibble
        lcd_tempbyte=text[lcd_j]&0xF0; // Write Most Significant Nibble of text[j] (LCD in 4-bit mode)
        sbs[6*lcd_j]=(lcd_tempbyte|0x09); // Least Significant Nibbles for I2C data = 9-D-9 for Data
        sbs[6*lcd_j+1]=(lcd_tempbyte|0x0D);
        sbs[6*lcd_j+2]=(lcd_tempbyte|0x09);
        // Send lower nibble
        lcd_tempbyte=text[lcd_j]<<4; // Write Most Significant Nibble of text[j] (LCD in 4-bit mode)
        sbs[6*lcd_j+3]=(lcd_tempbyte|0x09); // Least Significant Nibbles for I2C data = 9-D-9 for Data
        sbs[6*lcd_j+4]=(lcd_tempbyte|0x0D);
        sbs[6*lcd_j+5]=(lcd_tempbyte|0x09);
    }
    i2c_write_sbs(nin);
}

void lcdxy(char x, char y){ 
    uint8_t address=0;
    switch(y) {
        case 1:
            address = LINE_1_ADDRESS;
            break;
        case 2:
            address = LINE_2_ADDRESS;
            break;
        case 3:
            address = LINE_3_ADDRESS;
            break;
        case 4:
            address = LINE_4_ADDRESS;
            break;
        default:
            address = LINE_1_ADDRESS;
            break; 
    }
    address += x-1;
    LCD_Send_Byte(0, 0x80 | address);
}

void lcdinit(void){
    _I2C_Initialize();
    
    DLms(30); // Let LCD power up
                        // E bit must be bit 2 of low nibble first write is E low, 2nd is E high and 3rd is E low
                        // Command/Data bit is bit 0 of low nibble
                        // bit 3 of low nibble seems to be always HIGH, not sure its purpose
                        // all these low nibble bits must be used by pic processor on i2c board for how to set R/S RW and E pins on LCD when it sends 4 bits to LCD
    sbs[0]=(0x38); // Write Nibble 0x03 (per HD44780U initialization spec // Least Significant Nibbles for I2C COMMAND = 8-C-8 (
    sbs[1]=(0x3C);
    sbs[2]=(0x38);
    
    sbs[3]=(0x38); // Write Nibble 0x03 (per HD44780U initialization spec)
    sbs[4]=(0x3C); 
    sbs[5]=(0x38); 

    sbs[6]=(0x38); // Write Nibble 0x03 (per HD44780U initialization spec)
    sbs[7]=(0x3C); 
    sbs[8]=(0x38); 

    sbs[9]=(0x28); // Write Nibble 0x02 (per HD44780U initialization spec) 
    sbs[10]=(0x2C); 
    sbs[11]=(0x28); 

    sbs[12]=(0x28); // Set mode: 4-bit, 2+lines, 5x8 dots 
    sbs[13]=(0x2C); 
    sbs[14]=(0x28); 

    sbs[15]=(0x88); 
    sbs[16]=(0x8C); 
    sbs[17]=(0x88); 

    sbs[18]=(0x08); // Display ON: Write Byte 0x0C 
    sbs[19]=(0x0C);
    sbs[20]=(0x08);

    sbs[21]=(0xC8);
    sbs[22]=(0xCC);
    sbs[23]=(0xC8);

    sbs[24]=(0x08); // Clear Display: Write Byte 0x01 
    sbs[25]=(0x0C);
    sbs[26]=(0x08);

    sbs[27]=(0x18);
    sbs[28]=(0x1C);
    sbs[29]=(0x18);

    sbs[30]=(0x08); // Increment cursor: Write Byte 0x06
    sbs[31]=(0x0C);
    sbs[32]=(0x08);

    sbs[33]=(0x68);
    sbs[34]=(0x6C);
    sbs[35]=(0x68);
    
    i2c_write_sbs(36);
}

void lcdbanner(void) {
    lcdcs();
    lcdxy(1,1);
    lcdtext("DEV Board V2.1"); // write name of project to LCD
}