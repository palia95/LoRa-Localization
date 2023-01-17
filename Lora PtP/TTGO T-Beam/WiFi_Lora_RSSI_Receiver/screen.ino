/*

SSD1306 - Screen module

Copyright (C) 2018 by Xose Pérez <xose dot perez at gmail dot com>


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Wire.h>
#include "SSD1306Wire.h"
#include "OLEDDisplay.h"
#include "images.h"
#include "fonts.h"

#define SCREEN_HEADER_HEIGHT    14

SSD1306Wire * display;
uint8_t _screen_line = SCREEN_HEADER_HEIGHT - 1;

void screen_off() {
    display->displayOff();
}

void screen_on() {
    display->displayOn();
}

void screen_clear() {
    display->clear();
}

void screen_print() {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->clear();
    display->drawString(0, 0, "Received "+ packSize + " bytes");
    display->drawString(0, 9, packet);
    display->drawString(0, 18, "RSSI "+String(rssi)+" dBm");
    display->drawString(0, 27, "SNR "+String(snr));
    if (est_distance==0){
      display->drawString(0, 36, "Calibrating Reference RSSI");
    }
    else{
      display->drawString(0, 36, "Est distance: "+ String(est_distance) + " m");
      display->drawString(0, 45, "AVG RSSI "+String(avg_rssi)+" dBm");
    }
    display->display();
}


void screen_update() {
    display->display();
}

void screen_setup() {
    // Display instance
    display = new SSD1306Wire(SSD1306_ADDRESS, I2C_SDA, I2C_SCL);
    display->init();
    display->flipScreenVertically();
    display->setFont(Custom_ArialMT_Plain_10);
    screen_on();
}
