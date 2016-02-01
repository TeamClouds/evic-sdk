/*
 * This file is part of eVic SDK.
 *
 * eVic SDK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eVic SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eVic SDK.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2016 ReservedField
 * Copyright (C) 2016 kfazz
 */

#ifndef EVICSDK_ATOMIZER_H
#define EVICSDK_ATOMIZER_H

/**
 * Maximum output voltage, in millivolts.
 */
#define ATOMIZER_MAX_VOLTS 9000

/**
 * Initializes the atomizer library.
 * System control registers must be unlocked.
 */
void Atomizer_Init();

/**
 * Sets the atomizer output voltage.
 * This can always be called, the atomizer doesn't need
 * to be powered on.
 * While the voltage is expressed in mV, the actual precision
 * is to 1/100 of a Volt. Millivolt units are used for uniformity
 * between SDK functions.
 *
 * @param volts Output voltage, in millivolts.
 */
void Atomizer_SetOutputVoltage(uint16_t volts);

/**
 * Powers the atomizer on or off.
 *
 * @param powerOn True to power the atomizer on, false to power it off.
 */
void Atomizer_Control(uint8_t powerOn);

/**
 * Checks whether the atomizer is powered on.
 *
 * @return True if the atomizer is powered on, false otherwise.
 */
uint8_t Atomizer_IsOn();

/**
 * Reads the atomizer resistance.
 * If the atomizer is powered off, this powers it on for a
 * few milliseconds at a very low voltage, in order to
 * measure its resistance. Because of this, avoid continually
 * calling it while not firing.
 *
 * @return Atomizer resistance, in milliohms.
 */
uint16_t Atomizer_ReadResistance();

#endif