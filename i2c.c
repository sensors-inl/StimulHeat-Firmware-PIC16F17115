/*
 * File:   i2c.c
 * Author: MESNAGE Matthieu
 */

#include "xc.h"

/**************************************************************************/
/*!
   @brief    Initialise I2C MASTER.
*/
/**************************************************************************/
void I2C_MASTER_Init(void){
    
    SSP1CON1 = 0b00101000;            //SSP Module as Master
    SSP1CON2 = 0;
    SSP1ADD = 0x9F; //Setting Clock Speed
    SSP1STAT = 0;
    
    //DATASHEET : If a pin also has analog functions associated, the ANSEL bit for that pin must be cleared to enable the digital input buffer
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    WPUAbits.WPUA0 = 1;
    WPUAbits.WPUA1 = 1;
}

/**************************************************************************/
/*!
   @brief    Initialise I2C SLAVE.
*/
/**************************************************************************/
void I2C_SLAVE_Init(uint8_t addr)
{
  /*SSP1STAT = 0x80;
  SSP1ADD = (addr<<1) & 0xFF;
  SSP1CON1 = 0x36;
  SSP1CON2 = 0x01;*/
    
    /* CKE disabled; SMP Standard Speed;  */
    SSP2STAT = 0xA4;
    /* SSPM 7 Bit Polling; CKP disabled; SSPEN disabled; SSPOV no_overflow; WCOL no_collision;  */
    SSP2CON1 |= 0x6;
    /* SEN enabled; RSEN disabled; PEN disabled; RCEN disabled; ACKEN disabled; ACKDT acknowledge; GCEN disabled;  */
    SSP2CON2 = 0x1;
    /* DHEN disabled; AHEN disabled; SBCDE disabled; SDAHT 100ns; BOEN disabled; SCIE disabled; PCIE disabled;  */
    SSP2CON3 = 0x0;
    /* SSPADD 0;  */
    SSP2ADD = 0x0;
    /* SSPMSK 0;  */
    SSP2MSK = 0x0;
  
  //DATASHEET : If a pin also has analog functions associated, the ANSEL bit for that pin must be cleared to enable the digital input buffer
  ANSELAbits.ANSA0 = 0;
  ANSELAbits.ANSA1 = 0;
  TRISAbits.TRISA0 = 1;
  TRISAbits.TRISA1 = 1;
  WPUAbits.WPUA0 = 1;
  WPUAbits.WPUA1 = 1;
  
  GIE = 1;
  PEIE = 1;
  /*SSP1IF = 0;
  SSP1IE = 1;*/
  
    PIE5bits.SSP2IE = 1;
    SSP2IF = 0;
    PIE5bits.BCL2IE = 1;
    SSP2CON3bits.PCIE = 1;
    SSP2CON1bits.SSPEN = 1;
}

/**************************************************************************/
/*!
   @brief    Wait I2C Communication.
*/
/**************************************************************************/
void I2C_MASTER_Wait(void)
{
  while ((SSP2STAT & 0x04) || (SSP2CON2 & 0x1F)); //Transmit is in progress
}

/**************************************************************************/
/*!
   @brief    Start I2C Communication.
*/
/**************************************************************************/
void I2C_MASTER_Start(void)
{
  I2C_MASTER_Wait();    
  SSP2CON2bits.SEN = 1;             //Initiate start condition
}

/**************************************************************************/
/*!
   @brief    Restart I2C Communication.
*/
/**************************************************************************/
void I2C_MASTER_RepeatedStart(void)
{
  I2C_MASTER_Wait();
  SSP2CON2bits.RSEN = 1;           //Initiate repeated start condition
}

/**************************************************************************/
/*!
   @brief    Stop I2C Communication.
*/
/**************************************************************************/
void I2C_MASTER_Stop(void)
{
  I2C_MASTER_Wait();
  SSP2CON2bits.PEN = 1;           //Initiate stop condition
}

/**************************************************************************/
/*!
   @brief    Write I2C Communication.
*/
/**************************************************************************/
void I2C_MASTER_Write(uint8_t d)
{
  I2C_MASTER_Wait();
  SSP2BUF = d;         //Write data to SSPBUF
}