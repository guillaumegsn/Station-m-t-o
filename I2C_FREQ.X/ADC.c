/*
 * File:   ADC.c
 * Author: Guillaume
 *
 * Created on 27 septembre 2022, 12:14
 */


#include <xc.h>
#include "ADC.h"
#include "LCD_IIC.h"

void ADC_Init(void){
    ADCON1bits.ADFM = 1;     //complement du registre H
    ADCON1bits.ADCS = 0b110; //Parametrage a Fosc/64
    ADCON1bits.ADPREF = 0b11;//Parametrage en mode refInt
    
    ANSELCbits.ANSC2 = 1;    //Mode analogique des broches RC2 et RC3
    ANSELCbits.ANSC3 = 1;
    
    TRISCbits.TRISC2 = 1;    //Mode entrée des broches RC2 et RC3
    TRISCbits.TRISC3 = 1;
    
    FVRCONbits.FVREN = 1;    //Activation de la reference de tension interne
    FVRCONbits.ADFVR = 0b11; //Ref interne a 4,096V
}

float ADC_Conv(char Broche){
    ADCON0bits.CHS = Broche;                    //On selectionne la broche de l'ADC
    ADCON0bits.ADON = 1;                        //On active l'ADC
    ADCON0bits.GO_nDONE = 1;                    //On lance une conversion
    while (ADCON0bits.GO_nDONE);                //On attend la fin d'une conversion
    int result = (int)(ADRESH<<8) + (int)ADRESL;//On recupere le resultat dans un entier
    float Tens = (float)result * (float)0.004;  //On convertis le resultat en tension
    ADCON0bits.ADON = 0;                        //On desactive l'ADC
    return Tens;                                //On retourne le resultat en tension
}

void ADC_Afficher_Tension(char LCD_ADD,float MesF){
    /*Isolation des digits*/
        char dec = (char)MesF;
        afficherCaractereLCD(LCD_ADD, dec + 48); //On envoi le caractere + 48 pour obtenir le code ASCII
        afficherCaractereLCD(LCD_ADD, ',');
        char dix = (char)((MesF - dec) * 10);
        afficherCaractereLCD(LCD_ADD, dix + 48);
        char cen = (char)(((MesF - dec)*10 - dix) * 10);
        afficherCaractereLCD(LCD_ADD, cen + 48);
        char mil = (char)((((MesF - dec)*10 - dix) * 10 - cen)*10);
        afficherCaractereLCD(LCD_ADD, mil + 48);
        afficherCaractereLCD(LCD_ADD, 'V');
}
//float ADC_Conv_Temp(float Mes_Tem) {
  //  float Val_C = ((Mes_Tem - (float)2.7315)/(float)0.01);
    //return Val_C;
//}
