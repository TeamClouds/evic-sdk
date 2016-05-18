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
 */

#include <M451Series.h>
#include <Battery.h>
#include <ADC.h>

/**
 * \file
 * Battery library.
 * Voltage reading is done through the ADC.
 * PD.7 is low when the battery is present.
 */

/**
 * Battery mV voltage to percent lookup table.
 * percentTable[i] maps the 10% range starting at 10*i%.
 * Linear interpolation is done inside the range.
 */
static const uint16_t Battery_percentTable[11] = {
	3100, 3300, 3420, 3500, 3580, 3630, 3680, 3790, 3890, 4000, 4100
};

/**
 * True if battery is present, false otherwise.
 */
static volatile uint8_t Battery_isPresent;

/**
 * PD.7 interrupt handler. Needed to make use of debounce.
 * Dirty hack: not an actual interrupt handler, but will be
 * called for PD.7 by the real GPD interrupt handler that
 * resides in the button library.
 * This is an internal function.
 */
void GPD7_IRQHandler() {
	Battery_isPresent = !PD7;
}

void Battery_Init() {
	// Setup presence pin with debounce
	GPIO_SetMode(PD, BIT7, GPIO_MODE_INPUT);
	GPIO_ENABLE_DEBOUNCE(PD, BIT7);

	Battery_isPresent = !PD7;

	// Leave NVIC enable to the button library
	GPIO_EnableInt(PD, 7, GPIO_INT_BOTH_EDGE);
}

uint8_t Battery_IsPresent() {
	return Battery_isPresent;
}

uint8_t Battery_IsCharging() {
	return Battery_IsPresent() && USBD_IS_ATTACHED();
}

uint16_t Battery_GetVoltage() {
	uint8_t i;
	uint16_t adcSum;

	// Sample and average battery voltage.
	// A power-of-2 sample count is better for
	// optimization. 16 is also the biggest number
	// of samples that will fit in a uint16_t.
	adcSum = 0;
	for(i = 0; i < 16; i++) {
		adcSum += ADC_Read(ADC_MODULE_VBAT);
	}

	// Double the voltage to compensate for the divider
	return adcSum / 16L * 2L * ADC_VREF / ADC_DENOMINATOR;
}

uint8_t Battery_VoltageToPercent(uint16_t volts) {
	uint8_t i;
	uint16_t lowerBound, higherBound;

	// Handle corner cases
	if(volts <= Battery_percentTable[0]) {
		return 0;
	}
	else if(volts >= Battery_percentTable[10]) {
		return 100;
	}

	// Look up higher bound
	for(i = 1; i < 10 && volts > Battery_percentTable[i]; i++);

	// Interpolate
	lowerBound = Battery_percentTable[i - 1];
	higherBound = Battery_percentTable[i];
	return 10 * (i - 1) + (volts - lowerBound) * 10 / (higherBound - lowerBound);
}
