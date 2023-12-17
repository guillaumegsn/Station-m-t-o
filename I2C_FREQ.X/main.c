/*
 * File:   main.c
 * Author: Guillaume
 *
 * Created on 14 septembre 2022, 21:44
 */


#include <xc.h>
#include "IIC.h"
#include "ADC.h"
#include "LCD_IIC.h"
#include "FREQ.h"
#define _XTAL_FREQ 2000000UL
#define LCD_ADD 0x20
#define RC2 0b00110
#define RC3 0b00111

unsigned long temps;
float freq;
int tmr1;
char ini, pg = 0, pgpres = 0, gate = 0, overFlow = 0, overFsave, tmr1h, tmr1l, mesure = 0;
char* str0 = "TENSION SUR RC2 :";
char* str1 = "TENSION SUR RC3 :";
char* str2 = "FREQ LUE SUR RA2 :";
char* pg1 = "1/2";
char* pg2 = "2/2";


void __interrupt() ISR(void){
    if(INTCONbits.IOCIE == 1 && INTCONbits.IOCIF == 1){
        if (IOCAF5 == 1){
            IOCAF5 = 0;
            pgpres = pg;
            pg = !pg;
        }
        // clear global interrupt-on-change flag
        INTCONbits.IOCIF = 0;
    }
    
    if(PIR1bits.TMR1GIF){
        PIR1bits.TMR1GIF = 0;
        //detection de la gate du timer
        gate = !gate;
        if (gate){
            TMR1H = 0;
            TMR1L = 0;
            overFlow = 0;
            mesure = 0;
            LATCbits.LATC4 = 1;
        } else {
            mesure = 1;
            tmr1h = TMR1H;
            tmr1l = TMR1L;
            T1CONbits.TMR1ON = 0;
            overFsave = overFlow;
            LATCbits.LATC4 = 0;
        }
    }
    if(PIR1bits.TMR1IF){
        PIR1bits.TMR1IF = 0;
        //compte des overflow
        overFlow++;
    }
    
    return;
}

void main(void) {
    ini = 1;
    while(1){          //Boucle infinie du main
        if(ini == 1){ //Boucle d'initialisation a execution unique
            
            /*Initialisation*/
            IntDisable();
            IIC_Init();
            ADC_Init();
            FREQ_init();
            /*Fin_Initialisation*/
            
            /*Configuration du LCD*/
            LCD_Begin(LCD_ADD);
            /*Fin_Configuration du LCD*/
            IntEnable();
            
            TRISCbits.TRISC4 = 0;
        }
        ini = 0;

        if(mesure){
            tmr1 = ((int)tmr1h << 8) + (int)tmr1l;
            temps = (unsigned long)((overFsave) * 65535);
            freq  = 1./((8./(float)_XTAL_FREQ) * ((float)tmr1 + (float)temps));
        }
        
        if (pg != pgpres){
            LCD_Clr(LCD_ADD);
        }
        if (pg == 0){
            /*Affichage de la tension lue sur RC2*/
            LCD_SetCursor(LCD_ADD, 1,1);
            LCD_Print(LCD_ADD, str0);
            LCD_SetCursor(LCD_ADD, 8,2);
            ADC_Afficher_Tension(LCD_ADD, ADC_Conv(RC2));
            /*Fin_Affichage de la tension lue sur RC2*/
            
            /*Affichage de la tension lue sur RC3*/
            LCD_SetCursor(LCD_ADD, 1,3);
            LCD_Print(LCD_ADD, str1);
            LCD_SetCursor(LCD_ADD, 8,4);
            ADC_Afficher_Tension(LCD_ADD, ADC_Conv(RC3));
            LCD_SetCursor(LCD_ADD, 18,4);
            LCD_Print(LCD_ADD, pg1);
            /*Fin_Affichage de la tension lue sur RC3*/
        } else if (pg == 1){
            /*Affichage de la frequence lue sur RA2*/
            LCD_SetCursor(LCD_ADD, 1,1);
            LCD_Print(LCD_ADD, str2);
            LCD_SetCursor(LCD_ADD, 6,2);
            FREQ_Afficher(LCD_ADD, freq);
            LCD_SetCursor(LCD_ADD, 18,4);
            LCD_Print(LCD_ADD, pg2);
            /*Fin_Affichage de la frequence lue sur RA2*/
            T1CONbits.TMR1ON = 1;
        }
        __delay_ms(500);
        
    }
    return;
}
    