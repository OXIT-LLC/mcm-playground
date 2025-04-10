/**
 * @file led_control.h
 * @author Noman Cyclewala
 * @brief Header file for controlling an Adafruit NeoPixel LED.
 * @version 0.1
 * @date 2025-03-21
 *
 * Copyright (c) 2024 Oxit.
 * All rights reserved.
 * 
 * THE OPEN SOURCE SOFTWARE LICENSE AGREEMENT ("AGREEMENT") IS A BINDING LEGAL CONTRACT BETWEEN YOU ("YOU") AND OXIT, A COMPANY INCORPORATED UNDER THE LAWS OF THE UNITED STATES OF AMERICA ACTING FOR THE PURPOSE OF THIS AGREEMENT THROUGH ITS REGISTERED OFFICE AT OXIT, LLC, 3131 WESTINGHOUSE BLVD, CHARLOTTE, NC 28273.
 * 
 * THIS SOFTWARE LICENSE AGREEMENT ("AGREEMENT") GOVERNS YOUR USE OF THE MCM PLAYGROUND SOFTWARE. INSTALLING, COPYING OR OTHERWISE USING THE SOFTWARE INDICATES YOUR ACCEPTANCE OF THE TERMS OF THIS AGREEMENT REGARDLESS OF WHETHER YOU CLICK THE "ACCEPT" BUTTON.
 * 
 * The Licensee is permitted to use this Software, provided the following conditions are met:
 * 1. Oxit hereby grants to Licensee a perpetual, no-charge, royalty free, copyright license to use, copy, modify  the software,  to prepare a Derivative Works based on the software and Utilize the software for personal, commercial, or industrial purposes.
 * 
 * 2.  Neither the name of Oxit or the name of its contributors to be used in order to promote the product developed out of this software without prior written permission.
 * 
 * 3. If the Licensee makes any bug fixes, workarounds, improvements, or corrections to the Software, the Licensee agrees to  provide Oxit with the necessary source code and documentation at no cost, allowing Oxit to incorporate these changes into the Oxit Software.
 * 
 * 4. Oxit has no obligation to provide any maintenance, support or updates for the software package
 * 
 * 5. If the software contains any Third Party Software, all use of such Third Party Software shall be subject to the terms of  the license from such third party. You agree to comply with all terms and conditions for use of Third Party Software.
 * 
 * 6.  Oxit does not make any endorsements or representations concerning Third Party Software and disclaims all implied warranties concerning Third Party Software. Third Party Software is offered "AS IS."
 * 
 * 7. Oxit does not claim for meeting any specific functional requirement of the Licensee. Oxit does not take any responsibility for the uninterrupted or the error free operation of Software.
 * 
 * 8. Oxit makes no guarantee that the Software is free from bugs, viruses, or other defects.
 * 
 * 9. The Software is provided to kick start development on the Oxit MCM DevKit. By using this Software, the Licensee agrees to take full responsibility for any damages that may occur to their product.
 * 
 * 10. This software with or without modifications to be used only with Oxtech MCM DevKit
 * 
 * WARRANTY DISCLAIMER
 * 
 * THIS SOFTWARE IS PROVIDED BY OXIT "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL OXIT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES SUCH AS (BUT NOT LIMITED TO) LOSS OF BUSINESS REVENUES, PROFITS OR SAVINGS OR LOSS OF DATA RESULTING  FROM THE USE OR INABILITY TO USE THE SOFTWARE. THE OXIT DOES NOT WARRANT FOR ANY NON-INFRINGEMENT REGARDING THIRD-PARTY INTELLECTUAL  PROPERTY RIGHTS. OXIT DISCLAIMS ALL LIABILITY FOR DAMAGES CAUSED BY THIRD PARTIES, INCLUDING MACILICOUS USE OF, OR INTEFERENCE WITH TRANSMISSION OF LICENSEE'S DATA.
 */

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @def NUMPIXELS
 * @brief Number of NeoPixels in the strip.
 *
 * Adjust this value based on the number of NeoPixels you are using.
 */
#define NUMPIXELS 1 // Number of Neo pixels

/**
 * @def NEOPIXEL_PIN
 * @brief Arduino pin connected to the NeoPixel data line.
 *
 * This pin is used to send data to the NeoPixel.
 */
#define NEOPIXEL_PIN 33 // Arduino pin connected to NeoPixel data line

/**
 * @def NEOPIXEL_POWER_PIN
 * @brief Arduino pin connected to the NeoPixel power.
 *
 * This pin is used to control power to the NeoPixel.
 */
#define NEOPIXEL_POWER_PIN 21 

// Primary Colors
#define NEO_PIXEL_RED_COLOR    pixels.Color(255, 0, 0)
#define NEO_PIXEL_GREEN_COLOR  pixels.Color(0, 255, 0)
#define NEO_PIXEL_BLUE_COLOR   pixels.Color(0, 0, 255)

// Secondary Colors
#define NEO_PIXEL_YELLOW_COLOR pixels.Color(255, 255, 0)
#define NEO_PIXEL_ORANGE_COLOR pixels.Color(255, 165, 0)
#define NEO_PIXEL_CYAN_COLOR   pixels.Color(0, 255, 255)
#define NEO_PIXEL_MAGENTA_COLOR pixels.Color(255, 0, 255)

// Other Colors
#define NEO_PIXEL_WHITE_COLOR  pixels.Color(255, 255, 255)
#define NEO_PIXEL_PURPLE_COLOR pixels.Color(128, 0, 128)
#define NEO_PIXEL_PINK_COLOR   pixels.Color(255, 20, 147)
#define NEO_PIXEL_AMBER_COLOR  pixels.Color(255, 191, 0)
#define NEO_PIXEL_GOLD_COLOR   pixels.Color(255, 215, 0)
#define NEO_PIXEL_TEAL_COLOR   pixels.Color(0, 128, 128)

// Shades of White
#define NEO_PIXEL_WARM_WHITE_COLOR pixels.Color(255, 244, 229)
#define NEO_PIXEL_COOL_WHITE_COLOR pixels.Color(240, 255, 255)


/**
 * @def NEO_PIXEL_BLINK_PERIOD_MS
 * @brief The period in milliseconds for blinking the NeoPixel.
 */
#define NEO_PIXEL_BLINK_PERIOD_MS 500

// extern Adafruit_NeoPixel pixels;

/**
 * @brief Initializes the LED control system.
 *
 * This function initializes the NeoPixel library and sets up the power pin.
 */
void led_control_init();

/**
 * @brief Blinks the NeoPixel with a specified color for a certain number of times.
 *
 * @param color The color to blink the NeoPixel.
 * @param times The number of times to blink the NeoPixel.
 * @param delay_time The delay in milliseconds between blinks.
 */
void blink_color(uint32_t color, int times, int delay_time);

/**
 * @brief Processes LED blinking state non-blockingly
 */
void process_blink_requests();

/**
 * @brief Indicates device boot-up sequence with RGB color cycling.
 */
void led_boot_up();

// Enum to handle different LED states
typedef enum 
{
    LED_JOINED_LORAWAN_NETWORK,
    LED_JOINED_SW_BLE_NETWORK,
    LED_JOINED_SW_FSK_NETWORK,
    LED_JOINED_SW_CSS_NETWORK,
    LED_DEVICE_NOT_CONNECTED,
    LED_SENDING_UPLINK,
    LED_SENDING_UPLINK_FAIL,
    LED_RECEIVED_DOWNLINK,
    LED_RECEIVE_DATA,
    LED_SEND_DATA,
    LED_SENSOR_READ_FAIL,
    LED_EEPROM_FAIL
} led_state_t;

/**
 * @brief Sets the LED state based on the provided enum value.
 *
 * This function controls the LED behavior for various device states.
 *
 * @param state The desired LED state.
 */
void set_led_state(led_state_t state);

#endif