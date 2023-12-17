/*
 * File:   IIC.c
 * Author: Guillaume
 *
 * Created on 16 septembre 2022, 17:05
 */


#include <xc.h>

void IIC_Init(){
        /*Configuration de l'horloge*/
        OSCCONbits.IRCF = 0b1100; //on met l'horloge a 2MHz

        /*Configuration des pins en sortie*/
        TRISCbits.TRISC0 = 0b0;
        TRISCbits.TRISC1 = 0b0;

        /*Mise a zero de toutes les broches*/
        LATC = 0x00;

        /*Mise en mode I/O des broches*/
        ANSELCbits.ANSC0 = 0b0;
        ANSELCbits.ANSC1 = 0b0;

        /*Passage des broches en mode open drain*/
        ODCONCbits.ODC0 = 0b1;
        ODCONCbits.ODC1 = 0b1;

        /*Configuration des pins en entrées*/
        TRISCbits.TRISC0 = 0b1;
        TRISCbits.TRISC1 = 0b1;

        /*Mise en mode 'default' du registre SSP1STAT*/
        SSP1STATbits.SMP = 0b1;

        /*Configuration de la liaison IIC*/
        SSP1CON1bits.SSPEN = 0b1;
        SSP1CON1bits.SSPM = 0b1000;

        SSPDATPPS = 0x11; //RC1->MSSP:SDA;
        SSPCLKPPS = 0x10; //RC0->MSSP:SCL;
        // including port assignments that MCC neglected
        RC0PPS = 0x10; //RC0->MSSP:SCL;
        RC1PPS = 0x11; //RC1->MSSP:SDA;

        /*Configuration de la frequence*/
        SSPADD = 0x04;
}

//SSP1STAT registre sur le status de la transmission

void IIC_Emis(char adr, char R_W, char data){
    /*Debut de transmission (start)*/
    SSP1CON2 |= 0x01;
    
    /*Attente de la fin du start*/
    while ((SSP1CON2 & 0x01) && (!(SSP1STAT & 0x08)));
    
    /*Envoi de l'adresse sur 7 bit + le bit R/W*/
    SSP1BUF = (adr << 1) + R_W;
    
    /*Attente de la fin de l'emission + Ack du slave*/
    while (SSP1STAT & 0x01);
    while (SSP1CON2bits.ACKSTAT == 1);
    
    /*Envoi de la donnée*/
    SSP1BUF = data;
    
    /*Attente de la fin de l'emission + Ack du slave*/
    while (SSP1STAT & 0x01);
    while (SSP1CON2bits.ACKSTAT == 1);
    
    /*Emission d'un stop*/
    SSP1CON2 |= 0x04;
}