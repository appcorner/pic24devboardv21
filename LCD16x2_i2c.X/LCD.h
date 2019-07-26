/*
 * File:   LCD.h
 * Author: anusorn
 *
 * Created on July 22, 2019, 8:09 AM
 */


#ifndef LCD_H
#define	LCD_H

void lcdinit(); //initializes LCD 4-bit mode 5x8 2lines(this is OK for 4 lines)
void lcdbanner(); //prints your banner test page on LCD
void lcdxy(char x, char y); //moves LCD cursor position to this (stay inside x and y limits!!!!)
void lcdcreatecustom(); //creates 8 custom characters you can display
void lcdcmd(uint8_t c); //sends a command byte to LCD
void lcddata(uint8_t c); //sends a data byte to LCD
void lcdcust(const char text[]); //prints a custom character on LCD at current xy position (adjust names and characters to your liking)
void lcdtext(const char text[]); //prints text on LCD at current xy position
void lcdcs(); //clears the LCD screen

#endif	//LCD_H