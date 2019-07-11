/*
 * File:   OLED.c
 * Author: anusorn
 *
 * Created on July 10, 2019, 4:02 PM
 */


#include <xc.h>

#include "i2c.h"

#include "tahoma font.h"
#include "tahoma.h"

volatile unsigned int         ColumnBf ;

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
    OLEDwrCmd(127); // Display Clock Divide Ratio / OSC Frequency 

    OLEDwrCmd(0xA8); // Set Multiplex Ratio
    OLEDwrCmd(63); // Multiplex Ratio for 128x64 (64-1)

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
    OLEDwrCmd(0x00);  // 


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
    OLEDwrCmd(0x21) ; //set column
    OLEDwrCmd(0x00);  //start
    OLEDwrCmd(127);   //end
    // set page(Row) start and end 8 rows (64bits)
    OLEDwrCmd(0x22) ; //set page
    OLEDwrCmd(0x00);  //start
    OLEDwrCmd(7);     //end
    //write data 0 to clear screen ---
    i2c_start();
    i2c_write(0x78);  // begin of cmd/data
    i2c_write(0x40);  // data write
    //clear
    for(i=0;i<1024;i++) {i2c_write(0);}
    i2c_stop();
}

//----------------------------------------------
//------ 13 x 16 bit font  Full font available
//----------------------------------------------
void LedWI13(unsigned char a)
{unsigned char b,c;
 unsigned int d;
 b=a&0x0f;
 c=a&0xf0;
 d=b*13; 
 //row 0-1,2-3,4-5,6-7
 switch(c)
  {
   case 0x80: OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(1);break;
   case 0xc0: OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(3);break;
   case 0xd0: OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(5);break;
   default:   OLEDwrCmd(0x22); OLEDwrCmd(6); OLEDwrCmd(7);break;
  }
 //column =13 width
 OLEDwrCmd(0x21); OLEDwrCmd(d); OLEDwrCmd(d+12);
 ColumnBf=d;//save column  
}
void LedWD13(unsigned char dat)
{ unsigned int  b,c;
  unsigned char a,i ;
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   
   c=(dat-' ')*26;
   for(a=0;a<2;a++,c++)
    {b=c;
     for(i=0;i<13;i++,b+=2)
     { i2c_write(text13[b]);}
    }

   i2c_stop();
   b=ColumnBf+=13;//next column 
   OLEDwrCmd(0x21); OLEDwrCmd(b); OLEDwrCmd(b+12);
}
void LedWStr13(unsigned char a,unsigned char *dat)
{unsigned char i,j;
 unsigned int  colbf,b,c ;
 OLEDwrCmd(0x21); OLEDwrCmd(0); OLEDwrCmd(12);
 switch(a)
 {
   case 1:  OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(1);break;
   case 2:  OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(3);break;
   case 3:  OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(5);break;
   default: OLEDwrCmd(0x22); OLEDwrCmd(6); OLEDwrCmd(7);break;
  }
 colbf=0;
 while(*dat)
 {
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //

   c=(*dat-' ')*26;
   for(j=0;j<2;j++,c++)
    {b=c;
     for(i=0;i<13;i++,b+=2)
     { i2c_write(text13[b]);}
    }

   i2c_stop(); dat++;
   colbf+=13;//next start column 
   OLEDwrCmd(0x21); OLEDwrCmd(colbf); OLEDwrCmd(colbf+12);
 
 }
}

int ThaiLevel(unsigned char ch)
{
	if (ch>=216 && ch<=218) return -1;
	else if ((ch==209) || (ch>=212 && ch<=215)) return 1;
	else if (ch>=231 && ch<=238) return 2;
	else return 0;
}
void LedWStr13TH(unsigned char a, unsigned char *dat)
{unsigned char cw = 13; // char width
 unsigned char i,j;
 unsigned int  colbf,c;
 unsigned char chbuf[cw*2];
 switch(a)
 {
   case 1:  OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(1);break;
   case 2:  OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(3);break;
   case 3:  OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(5);break;
   default: OLEDwrCmd(0x22); OLEDwrCmd(6); OLEDwrCmd(7);break;
  }
 colbf=0;
 while(*dat)
 {
   OLEDwrCmd(0x21); OLEDwrCmd(colbf); OLEDwrCmd(colbf+cw-1);
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //
   c=(*dat-32)*cw*2;
   if (ThaiLevel(*dat)==0)
   {
     // copy font to buf
     for(j=0;j<cw*2;j++)
     {
       chbuf[j] = text13n[c+j];
       //chbuf[j] = text13[c+j];
     }
   } else {
     // merge font to buf
     for(j=0;j<2;j++)
     {
       for(i=0;i<cw;i++)
       { 
         if (j==0) chbuf[j+(i*2)] = (0xe0 & text13n[c+j+(i*2)]) | chbuf[j+(i*2)];
         else chbuf[j+(i*2)] = (0x0f & text13n[c+j+(i*2)]) | chbuf[j+(i*2)];
         //if (j==0) chbuf[j+(i*2)] = (0x1f & text13[c+j+(i*2)]) | chbuf[j+(i*2)];
         //else chbuf[j+(i*2)] = (0x80 & text13[c+j+(i*2)]) | chbuf[j+(i*2)];
       }
     }
   }
   for(i=0;i<cw;i++)
   { 
     i2c_write(chbuf[(i*2)+1]);
     //i2c_write(chbuf[(i*2)]);
   }
   for(i=0;i<cw;i++)
   { 
     i2c_write(chbuf[(i*2)]);
     //i2c_write(chbuf[(i*2)+1]);
   }
   i2c_stop(); dat++;
   if (ThaiLevel(*dat)==0)
   {
     colbf+=cw; //next start column 
   }
 }
}
//-------------------------------------------------
//    22 x 32(4 pages(row))  Only English font
//-------------------------------------------------
//------------------------------------
void LedWI22(unsigned char a)
{unsigned char b ;
 unsigned int d;
 b=a&0x0f;
 d=b*22; 
 //row 0-3 , 4-7
 switch(a)
  {case 0x80: OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(3);break;
   default:   OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(7);break;
  }
 //column =22 width
 OLEDwrCmd(0x21); OLEDwrCmd(d); OLEDwrCmd(d+21);
 ColumnBf=d;//save column  
}
void LedWD22(unsigned char dat)
{ unsigned int b,c;
  unsigned char i,j ;
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //
   c=(dat-' ')*88;
   for(j=0;j<4;j++,c++)
    {b=c;
     for(i=0;i<22;i++,b+=4)
     { i2c_write(text22[b]);}
    } 

   i2c_stop();
   b=ColumnBf+=22;//next column 
   OLEDwrCmd(0x21); OLEDwrCmd(b); OLEDwrCmd(b+21);
}
void LedWStr22(unsigned char a,unsigned char *dat)
{unsigned char i,j;
 unsigned int  colbf,b,c ;
 OLEDwrCmd(0x21); OLEDwrCmd(0); OLEDwrCmd(21);
 switch(a)
  { case 1:  OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(3);break;
    default: OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(7);break;
  }
 colbf=0;
 while(*dat)
 {
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //
   
   c=(*dat-' ')*88;
   for(j=0;j<4;j++,c++)
    {b=c;
     for(i=0;i<22;i++,b+=4)
     { i2c_write(text22[b]);}
    } 
   
  
   i2c_stop(); dat++;
   colbf+=22;//next start column 
   OLEDwrCmd(0x21); OLEDwrCmd(colbf); OLEDwrCmd(colbf+21);

   switch(a)
    { case 1: OLEDwrCmd(0x22); OLEDwrCmd(0); OLEDwrCmd(3);break;
      default:   OLEDwrCmd(0x22); OLEDwrCmd(4); OLEDwrCmd(7);break;
    }
 
 }
}   
//--------------------------------------------------
//
//--- 32 x 48 bit(6 rows) font  Only Number font
//
//--------------------------------------------------
void LedWI32(unsigned char a)
{unsigned char b  ;
 unsigned int d;
 b=a&0x0f;
 d=b*32; 
 //row 2-7 only  64 hight
 OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(7);
 //column =32 width
 OLEDwrCmd(0x21); OLEDwrCmd(d); OLEDwrCmd(d+31);
 ColumnBf=d;//save column  
}
void LedWD32(unsigned char dat)
{ unsigned int b,c;
  unsigned char i,j ;
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //
   c=(dat-' ')*192;
   for(j=0;j<6;j++,c++)
    {b=c;
     for(i=0;i<32;i++,b+=6)
     { i2c_write(text32[b]);}
    } 
  
   i2c_stop();
   b=ColumnBf+=32;//next column 
   OLEDwrCmd(0x21); OLEDwrCmd(b); OLEDwrCmd(b+31);
} 

void LedWStr32(unsigned char a,unsigned char *dat)
{unsigned char i,j;
 unsigned int  colbf,b,c ;
 OLEDwrCmd(0x21); OLEDwrCmd(0); OLEDwrCmd(31);
//row 2-7 only
 OLEDwrCmd(0x22); OLEDwrCmd(2); OLEDwrCmd(7);
 colbf=0;
 while(*dat)
 {
   i2c_start();
   i2c_write(0x78); //start instruction
   i2c_write(0x40); //write data
   //
   c=(*dat-' ')*192;
   for(j=0;j<6;j++,c++)
    {b=c;
     for(i=0;i<32;i++,b+=6)
     { i2c_write(text32[b]);}
    } 
   
   i2c_stop(); dat++;
   colbf+=32;//next start column 
   OLEDwrCmd(0x21); OLEDwrCmd(colbf); OLEDwrCmd(colbf+31);
 
 }
}  
//---------------------------
void numthai13(unsigned char a)
{	 switch(a)
	 {
	  case 0: LedWD13('ð'); break;
	  case 1: LedWD13('ñ'); break;
	  case 2: LedWD13('ò'); break;
	  case 3: LedWD13('ó'); break;
	  case 4: LedWD13('ô'); break;
	  case 5: LedWD13('õ'); break;
	  case 6: LedWD13('ö'); break;
	  case 7: LedWD13('÷'); break;
	  case 8: LedWD13('ø'); break;
	  case 9: LedWD13('ù'); break;
	 }
}
//------------------------------
void ShowPict(unsigned char *pic)
{unsigned int  i;
    i2c_start();
    i2c_write(0x78);  // begin of cmnd/data
    i2c_write(0x40);  // data write
    //write a picture
    for(i=0;i<1024;i++){ i2c_write(pic[i]);}
    i2c_stop();
}
//------------------------------------------------------------------------
