/**
 * @file ArduinoMultiprotocolExample.h
 * @author Paresh (paresh@oxit.com)
 * @brief Header file for the Arduino Multiprotocol Example
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

#ifndef ARDUINO_MULTIPROTOCOL_EXAMPLE_H
#define ARDUINO_MULTIPROTOCOL_EXAMPLE_H

#include <stdint.h>
#include "mcm_rover.h"

/******************************************************************************
 * PRIVATE MACROS AND DEFINES
 ******************************************************************************/
// Set default EVK type if not defined
#define EVK_TYPE_G2R2

// Pin configuration for the MCM module when host is ESP32S2/S3 Feather
#define TX_PIN 10
#define RX_PIN 9
#define RESET_PIN 14

// Uncomment the following lines for ESP32 host configuration
// #define TX_PIN 4
// #define RX_PIN 5

// LoRaWAN port number for sending uplink
#define LORAWAN_PORT 152

// Interval in seconds for sending sensor data as uplink
#define UPLINK_INTERVAL_SECONDS 60

// Timeout in seconds for no response after last sent uplink
#define UPLINK_NO_RESPONSE_TIMEOUT_SECONDS 60

// I2C interface configuration
#define I2C_POWER_PIN 7
#define I2C_SDA_PIN 3
#define I2C_SCL_PIN 4

// Push button configuration
// EUSART1 TX/RX pin configuration for Bootloader
#ifdef EVK_TYPE_G2R2
#define BUTTON_PIN 0
#elif defined(EVK_TYPE_G2R1)
#define BUTTON_PIN 15
#endif

#define BUTTON_DEBOUNCE_DELAY 1000

// MCM EVK User LED configuration
#define MCM_EVK_USER_LED 38

// RS485 interface configuration
#define PIN_RS485_EN 16
#define PIN_RS485_RX 18
#define PIN_RS485_TX 17
#define RS485 Serial2

// Manufacturing mode and version information
#define ENABLE_MANUFACTURING_MODE 0
#define HOST_APP_VERSION_MAJOR 0x00
#define HOST_APP_VERSION_MINOR 0x07
#define HOST_APP_VERSION_PATCH 0x00

/******************************************************************************
 * PRIVATE TYPEDEFS
 ******************************************************************************/

// Define system states
typedef enum {
    STATE_SET_CONNECT_MODE,
    STATE_JOIN_NETWORK,
    STATE_READ_SENSOR,
    STATE_SEND_UPLINK,
    STATE_UPLINK_STATUS,
    STATE_IDLE,
    STATE_NO_LORAWAN_CRED,
    STATE_FIRMWARE_UPDATE,
} system_state;

// Structure to hold the uplink data
typedef struct {
    uint16_t temp;
    uint16_t hum;
    uint16_t reboot_count;
} uplink_data_t;


/**
 * @brief Switches to the specified network mode.
 *
 * This function stops the current network, validates credentials if necessary, sets the new mode, and updates the state.
 * @param new_mode The new connection mode to switch to.
 */
void switch_protocol_mode(ConnectionMode new_mode);

/**
 * @brief Retrieves the current GPS timestamp in Unix format.
 *
 * This function attempts to get the current GPS time from the modem.
 * If successful, it returns the timestamp in Unix format (seconds since Jan 1 1970).
 * If unsuccessful, it returns 0 and prints an error message.
 *
 * @param gps_time Pointer to store the retrieved GPS timestamp
 * @return int 0 on success, non-zero on failure
 */
int get_gps_timestamp(uint32_t *gps_time);

/**
 * @brief Requests time synchronization with the LoRaWAN network
 *
 * This function sends a request to the MCM module to synchronize the device time
 * with the LoRaWAN network. It waits for the response and validates the synchronization
 * process.
 *
 * @return int 0 on successful time sync request, non-zero on failure
 */
int request_lorawan_time_sync(void);


/**
 * @brief Retrieves the last downlink statistics from the modem.
 *
 * This function fetches the last downlink statistics including protocol type,
 * RSSI, SNR, and timestamp. It prints the statistics to the serial console.
 *
 * @param p_last_dl_stats Pointer to store the last downlink statistics
 * @return int 0 on success, non-zero on failure
 */
int app_get_dl_stats(get_last_dl_stats_t *p_last_dl_stats);

/**
 * @brief query next uplink mtu from modem via a refresh command
 * 
 * @param mtu Pointer to store the retrieved MTU size
 * @return int 0 on success, non-zero on failure
 */

int app_queryNextUplink_mtu(uint16_t *mtu);

/**
 * @brief Retrieves the cached next uplink MTU size from the modem. Only queries the modem if the cached value is altered.
 *
 * @param[out] mtu Pointer to store the retrieved MTU size
 * @return int 0 on success, non-zero on failure
 */
int app_getCachedMaxUplink_mtu(uint16_t *mtu);

#endif // LRWAN_SIDEWALK_EX_H