#include <stdint.h>

#include <Atomizer.h>
#include <Display.h>

#include "display.h"
#include "font/font_vaporware.h"
#include "globals.h"
#include "helper.h"
#include "images/temperature.h"

struct menuItem wattSettingsOptions[] = {
    {
        .type = EXITMENU,
        .label = "watt",
    },
    {
        .type = END,
    }
};

struct menuDefinition wattSettings = {
    .name = "Display Settings",
    .font = FONT_SMALL,
    .cursor = "*",
    .prev_sel = "<",
    .next_sel = ">",
    .less_sel = "-",
    .more_sel = "+",
    .menuItems = &wattSettingsOptions,
};

void wattInit() {
	// set this initial value because we may be switching
	// from another mode that changes our watts.
    g.watts = 15000;
    g.volts = wattsToVolts(g.watts, g.atomInfo.resistance);
    Atomizer_SetOutputVoltage(g.volts);
}

void wattFire() {
    g.vapeCnt++;
    while (gv.fireButtonPressed) {
	// Handle fire button
	if (!Atomizer_IsOn() && g.atomInfo.resistance != 0
	    && Atomizer_GetError() == OK) {
	    // Power on
	    Atomizer_Control(1);
	}
	// Update info
	// If resistance is zero voltage will be zero
	Atomizer_ReadInfo(&g.atomInfo);

	g.newVolts = wattsToVolts(g.watts, g.atomInfo.resistance);

	if (g.newVolts != g.volts || !g.volts) {
	    if (Atomizer_IsOn()) {

		// Update output voltage to correct res variations:
		// If the new voltage is lower, we only correct it in
		// 10mV steps, otherwise a flake res reading might
		// make the voltage plummet to zero and stop.
		// If the new voltage is higher, we push it up by 100mV
		// to make it hit harder on TC coils, but still keep it
		// under control.
		if (g.newVolts < g.volts) {
		    g.newVolts = g.volts - (g.volts >= 10 ? 10 : 0);
		} else {
		    g.newVolts = g.volts + 100;
		}

	    }

	    if (g.newVolts > ATOMIZER_MAX_VOLTS) {
		g.newVolts = ATOMIZER_MAX_VOLTS;
	    }

	    g.volts = g.newVolts;

	    Atomizer_SetOutputVoltage(g.volts);
	}
	g.vapeCnt++;
	updateScreen(&g);
    }
    if (Atomizer_IsOn())
	Atomizer_Control(0);
    g.vapeCnt = 0;
}

void wattUp() {
    g.newVolts = wattsToVolts(g.watts + 100, g.atomInfo.resistance);
    if (g.newVolts <= ATOMIZER_MAX_VOLTS) {
	g.watts += 100;
	g.volts = g.newVolts;
    }
}

void wattDown() {
    if (g.watts >= 100) {
	g.watts -= 100;
	g.volts = wattsToVolts(g.watts, g.atomInfo.resistance);
    }
}

void wattDisplay(uint8_t atomizerOn) {
    char buff[9];
	getFloatingTenth(buff, g.watts);
	Display_PutText(0, 5, buff, FONT_LARGE);
	getString(buff, "W");
	Display_PutText(48, 2, buff, FONT_SMALL);
}

void wattBottomDisplay(uint8_t atomizerOn) {
    char buff[9];
	Display_PutPixels(0, 100, tempImage, tempImage_width, tempImage_height);

	printNumber(buff, CToDisplay(g.atomInfo.temperature));
	Display_PutText(24, 107, buff, FONT_MEDIUM);
}
