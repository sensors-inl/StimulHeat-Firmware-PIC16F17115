/*
 * Project: StimulHeat
 * Authors: Matthieu Mesnage, Bertrand Massot
 * Company : INSA Lyon - INL
 * Date: 2025-05-28
 *
 * This file is licensed under the GNU General Public License v3.0.
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * For commercial use or licensing inquiries, contact:
 * bertrand.massot@insa-lyon.fr
 */
 
#include "xc.h"
#include "pin.h"
#include "i2c.h"

#define _XTAL_FREQ 1000000

// Config
/*
#pragma config FEXTOSC = OFF
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled)
#pragma config CLKOUTEN = OFF
*/

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = EXTOSC    // Power-up Default Value for COSC bits->EXTOSC operating per FEXTOSC bits
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; I/O function on RA4
#pragma config VDDAR = HI    // VDD Range Analog Calibration Selection bit->Internal analog systems are calibrated for operation between VDD = 2.3V - 5.5V

// CONFIG2
#pragma config MCLRE = EXTMCLR    // Master Clear Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR
#pragma config PWRTS = PWRT_OFF    // Power-up Timer Selection bits->PWRT is disabled
#pragma config WDTE = OFF    // WDT Operating Mode bits->WDT disabled; SEN is ignored
#pragma config BOREN = ON    // Brown-out Reset Enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO    // Brown-out Reset Voltage Selection bit->Brown-out Reset Voltage (VBOR) set to 1.9V
#pragma config PPS1WAY = ON    // PPSLOCKED One-Way Set Enable bit->The PPSLOCKED bit can be set once after an unlocking sequence is executed; once PPSLOCKED is set, all future changes to PPS registers are prevented
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset

// CONFIG4
#pragma config BBSIZE = BB512    // Boot Block Size Selection bits->512 words boot block size
#pragma config BBEN = OFF    // Boot Block Enable bit->Boot Block is disabled
#pragma config SAFEN = OFF    // SAF Enable bit->SAF is disabled
#pragma config WRTAPP = OFF    // Application Block Write Protection bit->Application Block is not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block is not write-protected
#pragma config WRTC = OFF    // Configuration Registers Write Protection bit->Configuration Registers are not write-protected
#pragma config WRTSAF = OFF    // Storage Area Flash (SAF) Write Protection bit->SAF is not write-protected
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored.

// CONFIG5
#pragma config CP = OFF    // User Program Flash Memory Code Protection bit->User Program Flash Memory code protection is disabled


// Functions

void processCommand(uint8_t);

// Variables

short temp;                             // TEMP DATA

uint8_t dataReceived = 0;               // Contains the concatenation of direction and DAC Command
uint8_t commandeDAC = 0;                // Contains the value to be sent by the DAC to the MOSFET [0:127]
uint8_t sens = 1;                       // Contains the value of the direction [0:1] 

uint8_t writeRegister0x01 = 1;          // Register Number : 0x01
uint8_t enableWriteRegister0x01 = 0;    // Bit enabling the modification of the value of register 0x01.


int main(void) {
    
    //Initialisation of Pins
    pin_I2C_Configuration();
    pin_MUX_Configuration();
    
    //Initialisation of Protocols
    I2C_SLAVE_Init(1);
    
    //Initialisation of DAC
    DAC1CONbits.EN = 1;
    DAC1CONbits.REFRNG = 0;
    DAC1CONbits.OE = 0b10;
    DAC1CONbits.PSS = 0b00;
    DAC1CONbits.NSS = 0;        
    
    while(1){
       // Send the actual command on the Peltier Element
       DAC1DATL = commandeDAC;
       PORTAbits.RA4 = sens;
       PORTAbits.RA5 = ~sens;
    }
    
    return 0;
}

/**************************************************************************/
/*
   @brief    Process the command receive on I2C Bus.
     @param    dataTemp    int corresponding to the value to be processed
*/
/**************************************************************************/
void processCommand(uint8_t dataTemp){
    
    // Let's check if we have already sent the register on which we wanted to change the value 
        
        // Register 0x01 : Value of Command
        if(enableWriteRegister0x01){
            sens = dataTemp >> 7;
            commandeDAC = dataTemp & 0b01111111;
            enableWriteRegister0x01 = 0;
        }
    
    // Otherwise, let's decode the value of the register on which we are going to write  
        else{
            
            // Register 0x01 : Value of Command
            if(dataTemp==writeRegister0x01){
               enableWriteRegister0x01 = 1; 
            }
            // If nothing corresponds, disable all registers 
            else{
               enableWriteRegister0x01 = 0;
            }

        }
}

/**************************************************************************/
/*
   @brief    Interruption Function for I2C_Slave
*/
/**************************************************************************/
void __interrupt() I2C_Slave_Read(void)
{
  if(SSP2IF == 1)
  {
    SSP2CON1bits.CKP = 0;

    // Detect if there are collisions
    if ((SSP2CON1bits.SSPOV) || (SSP2CON1bits.WCOL))
    {
      temp = SSP2BUF;           // Read the previous value to clear the buffer
      SSP2CON1bits.SSPOV = 0;   // Clear the overflow flag
      SSP2CON1bits.WCOL = 0;    // Clear the collision bit
      SSP2CON1bits.CKP = 1;
    }
    
    // Detect if it's a write request
    if(!SSP2STATbits.D_nA && !SSP2STATbits.R_nW)
    {
      temp = SSP2BUF;
      SSP2CON1bits.CKP = 1;
      while(!SSP2STATbits.BF);
      dataReceived = SSP2BUF;
      processCommand(dataReceived);
      SSP2CON1bits.CKP = 1;
    }
    
    // Detect if it's a read request
    else if(!SSP2STATbits.D_nA && SSP2STATbits.R_nW)
    {
      temp = SSP2BUF;
      SSP2STATbits.BF = 0;
      SSP2BUF = 0;
      SSP2CON1bits.CKP = 1;
      while(SSP2STATbits.BF);
    }

    SSP2IF = 0;
    SSP2CON1bits.CKP = 1;
  }
}

