/*
 * File:   pin.c
 * Author: MESNAGE Matthieu
 */

#include "xc.h"

/**************************************************************************/
/*!
   @brief    Initialise the I2C Pin of the Controller.
*/
/**************************************************************************/
void pin_I2C_Configuration(void){
    
    // >> Relocalisation Pin on RA1 and RA0 for I2C communication
    
    //Unlock PPS registers and disable interrupts
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
    
    /***********************************/
    
    //PPS Input Selection
    SSP2DATPPS = 0x0; // RA0 -> SDA2
    SSP2CLKPPS = 0x1; // RA1 -> SCL2
    
    //PPS Output Selection
    RA0PPSbits.RA0PPS = 0x1E; // RA0 -> SDA2
    RA1PPSbits.RA1PPS = 0x1D; // RA1 -> SCL2
            
    /***********************************/
    
    //Lock PPS registers and enable interrupts
    PPSLOCK = 0x55; 
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
    INTCONbits.GIE = 1;
}

/**************************************************************************/
/*!
   @brief    Initialise the Multiplexer Pin of the Controller.
*/
/**************************************************************************/
void pin_MUX_Configuration(void){
    
    // Disable Comparator / ADC / Timers / CLKOUT
    CM1CON0bits.EN = 0;
    CM2CON0bits.EN = 0;
    ADCON0bits.ON = 0;
    T1CONbits.ON = 0;
    T3CONbits.ON = 0;
    
    // >> Use of RA5 for MUX Control
    
    // Initialisation Pin Control MUX
    TRISAbits.TRISA5 = 0;
    LATAbits.LATA5 = 0;
    ANSELAbits.ANSA5 = 0;
    
    // Valeur par défaut
    PORTAbits.RA5 = 0;
    
    // >> Use of RA4 for RED LED
    
    // Initialisation Pin Control MUX
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;
    ANSELAbits.ANSA4 = 0;
    
    // Valeur par défaut
    PORTAbits.RA4 = 0;
}
