/* 
 * File:   LCD_IIC.h
 * Author: Guillaume
 *
 * Created on 17 septembre 2022, 16:23
 */

void LCD_Envoi(char, char);
void instructionLCD(char, char);
void LCD_SetCursor(char, char, char);
void LCD_Print(char, char*);
void LCD_Begin(char);
void LCD_On(char);
void LCD_Off(char);
void LCD_ShiftL(char);
void LCD_ShiftR(char);
void LCD_Clr(char);
void afficherCaractereLCD(char, char);