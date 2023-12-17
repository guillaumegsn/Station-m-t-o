/*
 * File:   LCD_IIC.c
 * Author: Guillaume 
 *
 * Created on 17 septembre 2022, 16:23
 */


#include <xc.h>
#include "IIC.h"
#include "LCD_IIC.h"
#define _XTAL_FREQ 2000000UL

#define LCD_premiere           0x80
#define LCD_deuxieme           0xC0
#define LCD_troisieme          0x94
#define LCD_quatrieme          0xD4
#define LCD_effacer            0x01
#define LCD_retour_depart      0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E
#define LCD_BACKLIGHT          0x08
#define LCD_NOBACKLIGHT        0x00

#ifndef LCD_TYPE
   #define LCD_TYPE 2           // 0=5x7, 1=5x10, 2=2 lines
#endif

__bit RS;
char backlight_val = LCD_BACKLIGHT;

void LCD_Envoi(char add, char data){
    data |= RS;
    IIC_Emis(add, 0, ((data & 0xFB) | backlight_val));
    __delay_us(1);
    IIC_Emis(add, 0, ((data | 0x04) | backlight_val));
    __delay_us(1);
    IIC_Emis(add, 0, ((data & 0xFB) | backlight_val));
    __delay_us(100);
}

void instructionLCD(char add, char inst){
    RS = 0;
    LCD_Envoi(add, (inst & 0xF0));
    LCD_Envoi(add, (inst << 4) & 0xF0);
    if((inst == LCD_effacer) || (inst == LCD_retour_depart))
    __delay_ms(2);
}

void LCD_Begin(char add){

  LCD_Envoi(add, 0);

  __delay_ms(40);
  instructionLCD(add ,3);
  __delay_ms(5);
  instructionLCD(add ,3);
  __delay_ms(5);
  instructionLCD(add ,3);
  __delay_ms(5);
  instructionLCD(add ,LCD_retour_depart);
  __delay_ms(5);
  instructionLCD(add ,0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  instructionLCD(add ,LCD_TURN_ON);
  __delay_ms(50);
  instructionLCD(add ,LCD_effacer);
  __delay_ms(50);
  instructionLCD(add ,LCD_ENTRY_MODE_SET | LCD_retour_depart);
  __delay_ms(50);
}

void LCD_SetCursor(char add, char col, char lig){
  switch(lig)
  {
    case 2:
      instructionLCD(add, LCD_deuxieme + col - 1);
      break;
    case 3:
      instructionLCD(add, LCD_troisieme  + col - 1);
      break;
    case 4:
      instructionLCD(add, LCD_quatrieme + col - 1);
    break;
    default:      // case 1:
      instructionLCD(add, LCD_premiere  + col - 1);
  }

}

void LCD_Print(char add, char* Str){
  uint8_t i = 0;
  RS = 1;
  while(Str[i] != '\0')
  {
    LCD_Envoi(add, Str[i] & 0xF0);
    LCD_Envoi(add, (Str[i++] << 4) & 0xF0 );
  }
}
void afficherCaractereLCD(char adresse, char donnee){
    char donneeH = ( donnee & 0xF0 );       // quartet le plus haut à envoyer
    char donneeL = ( donnee << 4 ) & 0xF0;  // quartet le plus bas à envoyer
    IIC_Emis( adresse, 0, donneeH + 9 );    // envoi du quartet haut
    IIC_Emis( adresse, 0, donneeH  + 13 );// Activation du Enable
    IIC_Emis( adresse, 0, donneeH + 9 );
    IIC_Emis( adresse, 0, donneeL +9 );    // Idem avec quartet bas.
    IIC_Emis( adresse, 0, donneeL  + 13 );
    IIC_Emis( adresse, 0, donneeL + 9 );
    __delay_us(37);
    return;
}

void LCD_On(char add) {
  backlight_val = LCD_BACKLIGHT;
  LCD_Envoi(add, 0);
}

void LCD_Off(char add) {
  backlight_val = LCD_NOBACKLIGHT;
  LCD_Envoi(add, 0);
}

void LCD_ShiftL(char add) {
  instructionLCD(add, LCD_SHIFT_LEFT);
  LCD_Envoi(add, 0);
}

void LCD_ShiftR(char add) {
  instructionLCD(add, LCD_SHIFT_RIGHT);
  LCD_Envoi(add, 0);
}

void LCD_Clr(char add) {
  instructionLCD(add, LCD_effacer);
}