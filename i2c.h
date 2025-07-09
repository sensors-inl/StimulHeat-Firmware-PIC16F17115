/* 
 * File: i2c.h 
 * Author: MESNAGE Matthieu
 */

// This is a guard condition so that contents of this file are not included more than once. 
#ifndef I2C_H
#define	I2C_H

#include "xc.h"

//Master Functions
void I2C_MASTER_Init(void);
void I2C_MASTER_Wait(void);
void I2C_MASTER_Start(void);
void I2C_MASTER_RepeatedStart(void);
void I2C_MASTER_Stop(void);
void I2C_MASTER_Write(uint8_t);

//Slave Function
void I2C_SLAVE_Init(uint8_t);

#endif	/* I2C_H */