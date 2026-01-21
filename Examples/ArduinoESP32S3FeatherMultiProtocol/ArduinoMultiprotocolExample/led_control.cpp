/**
 * @file led_control.cpp
 * @brief This file contains the functions for controlling the LED.
 */
#include "led_control.h"

#define USE_BLOCKING_DELAY 1

static uint32_t solid_led_state;

// Initialize the NeoPixel object.
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Enum to differentiate the current LED pattern.
typedef enum 
{
    PATTERN_NONE,
    PATTERN_BLINK,
    PATTERN_BOOT_UP,
    PATTERN_SOLID
} pattern_type_t;

// Structure that holds the state of the currently active LED pattern.
typedef struct 
{
    pattern_type_t type;

    // Parameters for blink pattern (used by blink_color and related functions)
    uint32_t blinkColor;
    uint8_t totalBlinks;
    uint8_t remainingBlinks;
    uint32_t blinkInterval;
    uint32_t lastChangeTime;
    bool ledState;

    // Parameters for boot-up fade sequence.
    // The boot sequence is broken into three phases:
    //  Phase 0: Fade from red to blue (red decreases, blue increases)
    //  Phase 1: Fade from blue to green (blue decreases, green increases)
    //  Phase 2: Fade from green to red (green decreases, red increases)
    uint8_t bootPhase; // 0, 1, or 2
    uint8_t bootStep;  // current step (0-255)
    uint32_t bootLastStepTime;
    uint32_t bootStepInterval; // Time between steps (e.g., 4 ms)
} led_pattern_t; // typedef for led_pattern

// Global variable to store the current active pattern.
static led_pattern_t activePattern = {PATTERN_NONE};

/**
 * @brief Initializes the LED control system.
 *
 * This function sets up the NeoPixel LED, turning on its power,
 * initializing the library, clearing any previous state, and
 * displaying the cleared state.
 */
void led_control_init()
{
    pinMode(NEOPIXEL_POWER_PIN, OUTPUT);
    digitalWrite(NEOPIXEL_POWER_PIN, HIGH);
    pixels.begin();
    pixels.clear(); // Turn off all pixels.
    pixels.show();
}


/**
 * @brief Sets the LED to a solid color (non-blinking).
 *
 * After calling solid_led(), the LED will maintain the given color until
 * another pattern is started.
 *
 * @param color The color to display.
 */
void solid_led(uint32_t color)
{

    solid_led_state    = color;
    activePattern.type = PATTERN_SOLID;
    pixels.setPixelColor(0, color);
    pixels.show();
}

/**
 * @brief Starts a blink pattern with a given color.
 *
 * The pattern is non-blocking. After calling blink_color(), you must call
 * process_blink_requests() regularly (e.g. inside your loop) to update the LED state.
 */
void blink_color(uint32_t color, int times, int interval)
{
#ifdef USE_BLOCKING_DELAY
    // Simple blocking delay implementation
    for (int i = 0; i < times; i++)
    {
        pixels.setPixelColor(0, color);
        pixels.show();
        delay(interval);
        pixels.setPixelColor(0, 0); // Turn off the LED
        pixels.show();
        delay(interval);
    }
    
    /// Restore the solid LED state 
    solid_led(solid_led_state);

#else
    activePattern.type            = PATTERN_BLINK;
    activePattern.blinkColor      = color;
    activePattern.totalBlinks     = times;
    activePattern.remainingBlinks = times;
    activePattern.blinkInterval   = interval;
    activePattern.lastChangeTime  = millis();
    activePattern.ledState        = false; // start with OFF state

    // Force immediate first ON state.
    pixels.setPixelColor(0, color);
    pixels.show();
    activePattern.ledState         = true;
    activePattern.lastChangeTime   = millis();
#endif
}

/**
 * @brief Starts the boot-up fade sequence.
 *
 * This function configures a multi-phase color fade without using delay().
 * The process_blink_requests() function will update the fade sequence.
 */
void led_boot_up()
{
#ifdef USE_BLOCKING_DELAY
    // Blocking implementation for boot-up fade sequence
    for (int phase = 0; phase < 3; phase++)
    {
        for (int step = 0; step <= 255; step++)
        {
            uint8_t red = 0, green = 0, blue = 0;
            // Determine the current phase and calculate color.
            switch (phase)
            {
                case 0:
                    red  = 255 - step;
                    blue = step;
                    break;
                case 1:
                    blue  = 255 - step;
                    green = step;
                    break;
                case 2:
                    green = 255 - step;
                    red   = step;
                    break;
            }
            pixels.setPixelColor(0, pixels.Color(red, green, blue));
            pixels.show();
            delay(4); // Delay for the fade step
        }
    }
    // Turn off the LED after the boot sequence
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();

    /// Restore the solid LED state 
    solid_led(solid_led_state);
#else
    activePattern.type             = PATTERN_BOOT_UP;
    activePattern.bootPhase        = 0;
    activePattern.bootStep         = 0;
    activePattern.bootLastStepTime = millis();
    activePattern.bootStepInterval = 4; // 4ms per fade step; adjust as needed.
#endif
}

/**
 * @brief Processes the current LED pattern state in a non-blocking way.
 *
 * This function must be called regularly (e.g., in loop()) so that
 * blink and boot-up sequences progress based on elapsed time.
 */
void process_blink_requests()
{
    uint32_t now = millis();

    // Check if blocking delay is used
    if (USE_BLOCKING_DELAY)
    {
        return; // Do not take any action if blocking delay is used
    }

    switch (activePattern.type)
    {
        case PATTERN_BLINK: {
            // Check if we have completed all blink cycles.
            if (activePattern.remainingBlinks == 0)
            {
                /// Restore the solid pattern after blinking operations are done
                solid_led(solid_led_state);
                return;
            }
            // If the interval has passed, toggle the LED.
            if (now - activePattern.lastChangeTime >= activePattern.blinkInterval)
            {
                if (activePattern.ledState)
                {
                    // Turn off the LED and count one complete blink cycle.
                    pixels.setPixelColor(0, 0);
                    activePattern.remainingBlinks--;
                }
                else
                {
                    // Turn the LED on.
                    pixels.setPixelColor(0, activePattern.blinkColor);
                }
                pixels.show();
                activePattern.ledState       = !activePattern.ledState;
                activePattern.lastChangeTime = now;
            }
            break;
        }

        case PATTERN_BOOT_UP: {
            // Update the fade step if enough time has elapsed.
            if (now - activePattern.bootLastStepTime >= activePattern.bootStepInterval)
            {
                uint8_t step = activePattern.bootStep;
                uint8_t red = 0, green = 0, blue = 0;
                // Determine the current phase and calculate color.
                switch (activePattern.bootPhase)
                {
                    case 0:
                        // Fade from red (255,0,0) to blue (0,0,255)
                        red   = 255 - step;
                        blue  = step;
                        green = 0;
                        break;
                    case 1:
                        // Fade from blue (0,0,255) to green (0,255,0)
                        blue  = 255 - step;
                        green = step;
                        red   = 0;
                        break;
                    case 2:
                        // Fade from green (0,255,0) to red (255,0,0)
                        green = 255 - step;
                        red   = step;
                        blue  = 0;
                        break;
                    default:
                        break;
                }
                pixels.setPixelColor(0, pixels.Color(red, green, blue));
                pixels.show();

                activePattern.bootStep++;
                // If one phase is complete, move to the next phase.
                if (activePattern.bootStep >= 255)
                {
                    activePattern.bootStep = 0;
                    activePattern.bootPhase++;
                    // After finishing all three phases, finish the boot sequence.
                    if (activePattern.bootPhase > 2)
                    {
                        // Turn off the LED and reset the pattern.
                        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
                        pixels.show();
                        activePattern.type = PATTERN_NONE;
                    }
                }
                activePattern.bootLastStepTime = now;
            }
            break;
        }

        case PATTERN_SOLID:
            activePattern.type = PATTERN_NONE;
            break;

        case PATTERN_NONE:
        default:
            // No active LED pattern.
            break;
    }
}

// New function to handle LED states
void set_led_state(led_state_t state)
{
    switch (state)
    {
        case LED_JOINED_LORAWAN_NETWORK:
            solid_led(NEO_PIXEL_CYAN_COLOR);
            break;
        case LED_JOINED_SW_BLE_NETWORK:
            solid_led(NEO_PIXEL_YELLOW_COLOR);
            break;
        case LED_JOINED_SW_FSK_NETWORK:
            solid_led(NEO_PIXEL_PURPLE_COLOR);
            break;
        case LED_JOINED_SW_CSS_NETWORK:
            solid_led(NEO_PIXEL_COOL_WHITE_COLOR);
            break;
        case LED_DEVICE_NOT_CONNECTED:
            solid_led(NEO_PIXEL_RED_COLOR);
            break;
        case LED_SENDING_UPLINK:
            blink_color(NEO_PIXEL_BLUE_COLOR, 3, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_SENDING_UPLINK_FAIL:
            blink_color(NEO_PIXEL_RED_COLOR, 3, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_RECEIVED_DOWNLINK:
            blink_color(NEO_PIXEL_GREEN_COLOR, 3, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_RECEIVE_DATA:
            blink_color(NEO_PIXEL_GREEN_COLOR, 1, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_SEND_DATA:
            blink_color(NEO_PIXEL_GREEN_COLOR, 1, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_SENSOR_READ_FAIL:
            blink_color(NEO_PIXEL_YELLOW_COLOR, 3, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        case LED_EEPROM_FAIL:
            blink_color(NEO_PIXEL_RED_COLOR, 5, NEO_PIXEL_BLINK_PERIOD_MS);
            break;
        default:
            break;
    }
}
