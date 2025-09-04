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

// This is a guard condition so that contents of this file are not included more than once. 
#ifndef PIN_H
#define	PIN_H

#include "xc.h"

void pin_I2C_Configuration(void);
void pin_MUX_Configuration(void);

#endif	/* PIN_H */


