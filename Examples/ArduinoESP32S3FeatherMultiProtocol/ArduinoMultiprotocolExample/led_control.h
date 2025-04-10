/**
 * @file led_control.h
 * @brief Header file for controlling an Adafruit NeoPixel LED.
 *
 * This file defines the functions and configurations for controlling a NeoPixel LED
 * to indicate various device states and events.
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