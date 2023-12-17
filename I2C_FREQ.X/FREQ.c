/*
 * File:   FREQ.c
 * Author: Guillaume
 *
 * Created on 27 septembre 2022, 18:04
 */


#include <xc.h>
#include "FREQ.h"
#include "LCD_IIC.h"

void FREQ_init(void) {
    
    LATAbits.LATA2 = 0;
    ANSELAbits.ANSA2 = 0;
    WPUAbits.WPUA2 = 0;
    TRISAbits.TRISA2 = 1;
    LATAbits.LATA5 = 0;
    WPUAbits.WPUA5 = 0;
    TRISAbits.TRISA5 = 1;
    
    T1CONbits.T1CKPS = 0b01; //Pre diviseur par 2
    T1CONbits.TMR1ON = 1;    //Activation du timer
    
    T1GCONbits.TMR1GE = 1;   //Activation du declanchement par gate du timer
    T1GCONbits.T1GPOL = 1;   //Activation sur etat haut (front montant)
    T1GCONbits.T1GTM = 1;    //Mode "Bascule (Toggle)"
    T1GCONbits.T1GSS = 0b00;
    T1GPPS = 0b00000010;     //Configuration de la gate sur RA2
    
    PIE1bits.TMR1GIE = 1; //Actigation TMR1 Gate
    PIE1bits.TMR1IE = 1;  //Activation interuptions overflow TMR1
    
    PIR1bits.TMR1GIF = 0; //Gate interupt flag
    PIR1bits.TMR1IF = 0;  //Overflow flag
    
    //IOCAPbits.IOCAP2 = 1; //activation des interuptions sur RA2
    IOCAPbits.IOCAP5 = 1; //activation des interuptions sur RA5 (PgUp)
    INTCONbits.IOCIE = 1; //On active les interuptions sur changement d'etat
    INTCONbits.PEIE = 1; //On autorise les interuptions dues aux peripheriques
    
    return;
}

void IntEnable(){
    INTCONbits.GIE = 1; //Activation des interuptions globales
}
void IntDisable(){
    INTCONbits.GIE = 0; //Desactivation des interuptions globales
}

void FREQ_Afficher(char LCD_ADD,float FRE){
    /*Isolation des digits*/
    if(FRE >= 99.99){
        char* XX = "XX,XXHz";
        LCD_Print(LCD_ADD, XX);
    } else {
        char d1z = (char)(FRE/10);
        afficherCaractereLCD(LCD_ADD, d1z + 48); //On envoi le caractere + 48 pour obtenir le code ASCII
        char dec = (char)((FRE/10 - d1z) * 10);
        afficherCaractereLCD(LCD_ADD, dec + 48);
        afficherCaractereLCD(LCD_ADD, ',');
        
        char dix = (char)((((FRE/10) - d1z)*10 - dec) * 10);
        afficherCaractereLCD(LCD_ADD, dix + 48);
        char cen = (char)((((FRE/10 - d1z)*10 - dec) * 10 - dix)*10);
        afficherCaractereLCD(LCD_ADD, cen + 48);
        
        afficherCaractereLCD(LCD_ADD, 'H');
        afficherCaractereLCD(LCD_ADD, 'z');
    }
}