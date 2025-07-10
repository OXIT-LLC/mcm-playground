/**
 * @file mcm_lorawan.ino
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief This Sketch demonstrates the  connectivity of the mcm module.
 *        The module is  set to to connect to the lorawan network
 *        and then send uplink to the module.
 *        A temperature and humidity sensor is attached to the module
 *        Send the uplink every 30 seconds( This is configurable)
 *        Check the status of the last sent uplink
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

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <Arduino.h>
#include "mcm_rover.h"
#include "oxit_cli_app.h"
#include "oxit_nvs.h"
#include <Adafruit_NeoPixel.h>
#include "Adafruit_SHT4x.h"
#include "hal/uart_types.h"
#include "SPIFFS.h"
#include "ArduinoSidewalkExample.h"
#include "led_control.h"

/******************************************************************************
 * EXTERN VARIABLES
 ******************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/

MCM mcm(Serial1, TX_PIN, RX_PIN, RESET_PIN);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

/**
 * ╔═════════════════════════════════════════════════════════════════════════════╗
 * ║                         Lorawan Credentials                                 ║
 * ╚═════════════════════════════════════════════════════════════════════════════╝
 */
uint8_t dev_eui[]     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t join_eui[]    = {0xFF, 0xFF, 0xFF, 0xFf, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t network_key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t saved_dev_eui[8]      = {0};
uint8_t saved_join_eui[8]     = {0};
uint8_t saved_network_key[16] = {0};

static system_state currentState = STATE_NO_LORAWAN_CRED;

float temp = 0.0;
float hum  = 0.0;

uplink_data_t uplink_data;

static bool is_device_joined = false;

/**
 * @brief Device connection mode.
 *
 * Indicates the current connection mode used by the device.
 * The default value is ConnectionMode::CONNECTION_MODE_NC, which means no connection mode selected.
 */
ConnectionMode device_mode = ConnectionMode::CONNECTION_MODE_NC;

uint8_t is_device_have_valid_lorawan_credentials = 0;

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

// Add these global variables
volatile bool buttonPressed                = false;
volatile unsigned long lastButtonInterrupt = 0;
const unsigned long debounceTime           = 50; // 50ms debounce time

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/

/**
 * @brief Sets the connection mode of the MCM module.
 *
 * @param mode The connection mode to be set.
 */
static void set_mcm_connect_mode(ConnectionMode mode);

/**
 * @brief Joins the LoRaWAN network.
 *
 * @return True if successful, false otherwise.
 */
static bool initiate_network_join();

/**
 * @brief Reads the sensor values.
 *
 * @param[out] temperature A pointer to store the temperature value.
 * @param[out] humidity A pointer to store the humidity value.
 */
static void read_sensor(float *temperature, float *humidity);

/**
 * @brief Sends the uplink data.
 *
 * @param temperature The temperature value.
 * @param humidity The humidity value.
 *
 * @return True if successful, false otherwise.
 */
static bool send_uplink(float temperature, float humidity);

/**
 * @brief Handles the downlink data.
 */
static void handle_downlink();

/**
 * @brief Validates the LoRaWAN credentials.
 *
 * @param dev_eui The device EUI.
 * @param dev_eui_len The length of the device EUI.
 * @param join_eui The join EUI.
 * @param join_eui_len The length of the join EUI.
 * @param network_key The network key.
 * @param network_key_len The length of the network key.
 *
 * @return True if the credentials are valid, false otherwise.
 */
static bool validate_lorawan_creds(uint8_t *dev_eui, uint8_t dev_eui_len, uint8_t *join_eui, uint8_t join_eui_len, uint8_t *network_key, uint8_t network_key_len);

/**
 * @brief Checks if all the elements in the array are 0xFF.
 *
 * @param arr The array to be checked.
 * @param len The length of the array.
 *
 * @return True if all elements are 0xFF, false otherwise.
 */
bool is_all_ff(uint8_t *arr, uint8_t len);

void helper_print_hex_array(const uint8_t *arr, size_t len);

// This function handles the button press events, including debouncing and switching network modes.
static void handleButtonPress();


/**
 * @brief Checks the device connection status and updates the LED state accordingly.
 *
 */
static void check_device_connection();

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/

static void set_mcm_connect_mode(ConnectionMode mode)
{

    // set the connection mode lorawan or sidewalk
    mcm.set_connect_mode(mode);

    // for sidewalk we do not need the credentials
    if (ConnectionMode::CONNECTION_MODE_LORAWAN == mode)
    {
        // TODO:check the validity of lorawan credentials
        Serial.print("Device EUI: ");
        helper_print_hex_array(saved_dev_eui, 8);
        Serial.print("Join EUI: ");
        helper_print_hex_array(saved_join_eui, 8);
        Serial.print("Network key: ");
        helper_print_hex_array(saved_network_key, 16);
        // set credentials in case of lorawan
        mcm.set_lorawan_credentials(saved_dev_eui, saved_join_eui, saved_network_key);
    }
}

static bool initiate_network_join()
{
    Serial.println("Joining  network...\n");
    // connect to the network
    mcm.connect_network();
    return true;
}

static void read_sensor(float *temperature, float *humid)
{

    Serial.println("Reading sensor data...");
    sensors_event_t humidity, temp;
    if (sht4.getEvent(&humidity, &temp))

    {
        Serial.print("Temperature: ");
        Serial.print(temp.temperature);
        Serial.println(" degrees C");
        Serial.print("Humidity: ");
        Serial.print(humidity.relative_humidity);
        Serial.println("% rH");
    }
    else
    {
        Serial.println("Enable to read sensor data, using dummy values");
        temp.temperature           = 25.0;
        humidity.relative_humidity = 50.0;
    }

    *temperature = temp.temperature;
    *humid       = humidity.relative_humidity;
}

static bool send_uplink(float temperature, float humidity)
{
    bool uplink_done = false;

    do
    {
        if (mcm.is_connected() == false && (device_mode != ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE))
        {
            Serial.println("Device not connected to network.\n");
            break;
        }

        uint16_t uplink_mtu = 0;
        if(app_getCachedNextUplink_mtu(&uplink_mtu) != 0){
            Serial.println("Failed to get uplink mtu");
            break;
        }

        if (uplink_mtu == 0)
        {
            Serial.println("Invalid next uplink mtu");
            break;
        }
        set_led_state(LED_SENDING_UPLINK);
        // Code to send uplink with temperature and humidity data
        Serial.printf("Sending uplink: Temp = %.2f, Humidity = %.2f, Reboot counter = %d\r\n", temperature, humidity, uplink_data.reboot_count);

        uplink_data_t uplink_data;
        uplink_data.temp = (uint16_t)(temperature * 100);
        uplink_data.hum  = (uint16_t)(humidity * 100);
        // reboot counter is already assigned on bootup
        Serial.print("Uplink in hex: ");
        helper_print_hex_array((uint8_t *)&uplink_data, sizeof(uplink_data));

        mcm.send_uplink((uint8_t *)&uplink_data, sizeof(uplink_data), LORAWAN_PORT, MCM_UPLINK_TYPE::MCM_UPLINK_TYPE_CONF);

        uplink_done = true; // Assume success if we reach this point

    } while (0); // Loop only once

    // Return true if uplink is successful
    return uplink_done;
}

static void handle_downlink()
{
    // lets check if any download is available
    if (mcm.is_downlink_available())
    {
        set_led_state(LED_RECEIVED_DOWNLINK);

        Serial.println("--------------------Downlink available--------------------");
        // retrieve the downlink and meta data
        uint8_t received_data[BUFFER_SIZE];
        uint16_t received_len;
        int8_t rssi;
        int8_t snr;
        uint16_t seq_port;

        mcm.get_downlink_data(received_data, &received_len, &rssi, &snr, &seq_port);

        Serial.printf("Rssi: %d\r\n", rssi);
        Serial.printf("Snr: %d\r\n", snr);
        if (ConnectionMode::CONNECTION_MODE_LORAWAN == mcm.get_connect_mode())
        {
            // if mode is lorawan then its port
            Serial.printf("Lorawan port: %d\n", seq_port);
        }
        else
        {
            // if mode is sidewalk then its sidewalk sequence
            Serial.printf("Sidewalk sequence: %d\n", seq_port);
        }

        Serial.printf("Receivced payload Size: %d\n", received_len);
        Serial.println("Received Downlink data: ");
        helper_print_hex_array(received_data, received_len);
        Serial.printf("\n");

        Serial.println("----------------------------------------------------");
    }
}

bool is_all_ff(uint8_t *arr, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (arr[i] != 0xff)
        {
            return false;
        }
    }
    return true;
}

static bool validate_lorawan_creds(uint8_t *dev_eui, uint8_t dev_eui_len, uint8_t *join_eui, uint8_t join_eui_len, uint8_t *network_key, uint8_t network_key_len)
{
    bool return_value = false;
    do
    {
        if (dev_eui == NULL || join_eui == NULL || network_key == NULL)
        {
            Serial.println("Invalid credentials");
            break;
        }
        if (dev_eui_len != 8 || join_eui_len != 8 || network_key_len != 16)
        {
            Serial.println("Invalid credentials length");
            break;
        }

        if (true == is_all_ff(dev_eui, dev_eui_len))
        {
            Serial.println("Invalid dev_eui");
            break;
        }
        if (true == is_all_ff(join_eui, join_eui_len))
        {
            Serial.println("Invalid join_eui");
            break;
        }
        if (true == is_all_ff(network_key, network_key_len))
        {
            Serial.println("Invalid network_key");
            break;
        }

        return_value = true;

    } while (0);

    return return_value;
}

// ISR function
void IRAM_ATTR buttonISR()
{
    unsigned long currentTime = millis();
    if (currentTime - lastButtonInterrupt > debounceTime)
    {
        buttonPressed       = true;
        lastButtonInterrupt = currentTime;
    }
}

/**
 * @brief Switches to the specified network mode.
 *
 * This function stops the current network, validates credentials if necessary, sets the new mode, and updates the state.
 * @param new_mode The new connection mode to switch to.
 */
void switch_protocol_mode(ConnectionMode new_mode)
{
    // Check if switching to LoRaWAN and validate credentials
    if (new_mode == ConnectionMode::CONNECTION_MODE_LORAWAN && !is_device_have_valid_lorawan_credentials)
    {
        Serial.println("Invalid LoRaWAN credentials, not switching to LoRaWAN");
        return;
    }

    // Stop the current network connection
    mcm.stop_network();
    delay(500);

    set_led_state(LED_DEVICE_NOT_CONNECTED);
    is_device_joined = false;

    // Log the mode switching
    Serial.print("Switching to ");
    switch (new_mode)
    {
        case ConnectionMode::CONNECTION_MODE_LORAWAN:
            Serial.println("LoRaWAN");
            break;
        case ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE:
            Serial.println("Sidewalk BLE");
            break;
        case ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK:
            Serial.println("Sidewalk FSK");
            break;
        case ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS:
            Serial.println("Sidewalk CSS");
            break;
        default:
            Serial.println("No Connection (NC)");
            break;
    }

    // Set the new connection mode
    device_mode = new_mode;

    // Update the current state to set the new connection mode
    currentState = STATE_SET_CONNECT_MODE;
}

void helper_print_hex_array(const uint8_t *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        Serial.printf("0x%02X", arr[i]);
        if (i != len - 1)
        {
            Serial.printf(",", arr[i]);
        }
    }
    Serial.println();
}

static void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted successfully");
}

/******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/

void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println("Starting setup");

#if ENABLE_MANUFACTURING_MODE

    Serial.println("Enabling manufacturing mode");
    Serial.println("This mode helps in flashing the manufacturing file in mcm by not issuing any command to the mcm");
    Serial.println("Please disable the manufacturing mode manually after flashing the manufacturing file");
    Serial.println("To disable the manufacturing mode, set ENABLE_MANUFACTURING_MODE to 0 in the .ino file");

#else

    Serial.println("Starting setup");

    initSPIFFS();

    pinMode(MCM_EVK_USER_LED, OUTPUT);
    digitalWrite(MCM_EVK_USER_LED, HIGH);

    // initiating the neo pixel
    led_control_init(); // Initialize the LED control

    // Initiate the boot-up sequence for the NeoPixel
    led_boot_up();
    set_led_state(LED_DEVICE_NOT_CONNECTED); // Set LED state for not connected

    pinMode(I2C_POWER_PIN, OUTPUT);
    digitalWrite(I2C_POWER_PIN, HIGH);
    Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN);

    Serial.println("Adafruit SHT4x test");
    if (!sht4.begin())
    {
        Serial.println("Couldn't find SHT4x");
    }
    else
    {
        Serial.println("Found SHT4x sensor");
        Serial.print("SHT4x sensor Serial number 0x");
        Serial.println(sht4.readSerial(), HEX);
        sht4.setPrecision(SHT4X_HIGH_PRECISION);
        sht4.setHeater(SHT4X_NO_HEATER);
    }

    if (false == nvs_storage_init())
    {
        Serial.println("Failed to initialize NVS storage");
    }

    else
    {
        Serial.println("Successfully initialized NVS storage");
    }

    // Initialize the command line interface application
    // This sets up the command line interface with the application specific commands,.
    init_command_line_app();

    // Configure button pin with pull-up and attach ISR
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

    // Initialize peripherals and callback functions etc. related to serial interface with MCM.
    MCM_STATUS status = mcm.begin();

    if (MCM_STATUS::MCM_OK != status)
    {
        Serial.println("mcm begin failed");
    }
    // Enable/disable debug serial logs (logs will be available on the default serial port of the Arduino board being used)
    // Caution: Turning debug logs on can flood the serial logs
    mcm.set_debug_enabled(true);

    // reboot the module
    mcm.hw_reset();

    ver_type_1_t mcm_rover_lib_ver, c_lib_ver;
    mcm.retrieveLibraryVersions(&mcm_rover_lib_ver, &c_lib_ver);
    Serial.println("MCM Rover Library Version: " + String(mcm_rover_lib_ver.major) + "." + String(mcm_rover_lib_ver.minor) + "." + String(mcm_rover_lib_ver.patch));
    Serial.println("C Library Version: " + String(c_lib_ver.major) + "." + String(c_lib_ver.minor) + "." + String(c_lib_ver.patch));

    // set application version here
    ver_type_1_t host_ver;
    host_ver.major = HOST_APP_VERSION_MAJOR;
    host_ver.minor = HOST_APP_VERSION_MINOR;
    host_ver.patch = HOST_APP_VERSION_PATCH;
    mcm.set_host_app_version(host_ver);
    Serial.println("Host App version: " + String(host_ver.major) + "." + String(host_ver.minor) + "." + String(host_ver.patch));

    // get reset count from nvs flash
    uplink_data.reboot_count = nvs_storage_get_reboot_count();
    Serial.println("Reboot count: " + String(uplink_data.reboot_count));

    // Start in sidewalk BLE mode regardless of the validity of LoRaWAN credentials
    Serial.println("Starting in sidewalk BLE mode");
    currentState                             = STATE_SET_CONNECT_MODE;                       // Set to sidewalk mode
    is_device_have_valid_lorawan_credentials = 0;                                            // No valid credentials initially
    device_mode                              = ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK; // Set device mode to FSK

    // Check if credentials are available in the NVS storage
    if (true == nvs_storage_get_lorawan_cred(saved_network_key, saved_join_eui, saved_dev_eui))
    {
        Serial.println("Loaded saved lorawan credentials from nvs storage");
        is_device_have_valid_lorawan_credentials = true;
        // No need to switch to LoRaWAN mode
    }
    // If hardcoded credentials are valid
    else if (validate_lorawan_creds(dev_eui, sizeof(dev_eui), join_eui, sizeof(join_eui), network_key, sizeof(network_key)))
    {
        Serial.println("Loaded hardcoded credentials");
        memcpy(saved_dev_eui, dev_eui, sizeof(dev_eui));
        memcpy(saved_join_eui, join_eui, sizeof(join_eui));
        memcpy(saved_network_key, network_key, sizeof(network_key));
        // No need to switch to LoRaWAN mode
    }
    else
    {
        Serial.println("No valid credentials available, Please enter credentials manually for lorawan");
    }

    get_seg_file_status_t file_status;
    // send get segment command
    mcm.get_segmented_file_download_status(&file_status);

#endif
}

void loop()
{
#if ENABLE_MANUFACTURING_MODE
    // do nothing
#else
    process_blink_requests(); // Add this line to process LED states
    // Run the state machine to handle the current application state
    // This function checks the current state and performs the appropriate actions
    // based on the current state. This function is called repeatedly in the loop function.
    run_state_machine();

    // process the events received from MCM
    mcm.handle_rx_events();

    // handling the cli data from the command line
    process_command_line_app();

    // Call the handleButtonPress function to handle button press and debounce
    handleButtonPress();

#endif
}

/**
 * @brief Array of state names corresponding to the system_state enum.
 */
const char *state_names[] = {"STATE_SET_CONNECT_MODE", "STATE_JOIN_NETWORK",    "STATE_READ_SENSOR",          "STATE_SEND_UPLINK", "STATE_UPLINK_STATUS", "STATE_IDLE",
                             "STATE_NO_LORAWAN_CRED",  "STATE_FIRMWARE_UPDATE", "STATE_HOST_FIRMWARE_UPDATE", "UNKNOWN_STATE"};

/**
 * @brief Sets the current state of the system and prints the state name to the Serial monitor.
 *
 * @param state_value The new state to set.
 */
void set_state(system_state state_value)
{
    Serial.printf("Setting state to: %s\n", state_names[state_value]);
    currentState = state_value;
}

void run_state_machine()
{
    static uint32_t last_uplink_time = millis();

    // check for new binary file downloaded
    if (mcm.is_new_firmware())
    {
        // Check binary type from segment file status
        if (mcm.seg_file_status.cmd_type.bin_type == FUOTA_BINARY_TYPE_MCM)
        {
            Serial.println("New MCM firmware downloaded");
        }
        else if (mcm.seg_file_status.cmd_type.bin_type == FUOTA_BINARY_TYPE_HOST)
        {
            Serial.println("New Host firmware downloaded");
        }
        else
        {
            Serial.println("Unknown binary type downloaded");
        }
        set_state(STATE_FIRMWARE_UPDATE);
    }

    check_device_connection();

    // State machine logic
    switch (currentState)
    {
        case STATE_SET_CONNECT_MODE: {
            set_mcm_connect_mode(device_mode);
            set_state(STATE_JOIN_NETWORK);
            break;
        }

        case STATE_JOIN_NETWORK: {
            // Check for join failure
            // note: we are not taking join failure actions here, just printing the reason
            if (mcm.is_join_failure_available()) {
                uint8_t failure_reason;
                mcm.get_join_failure_info(&failure_reason);
                
                Serial.println("Join failure detected. Checking failure reasons:");
                
                if (failure_reason & JOIN_FAIL_REG) {
                    Serial.println("- Registration failure: Device registration failed");
                }
                if (failure_reason & JOIN_FAIL_TIME_SYNC) {
                    Serial.println("- Time sync failure: Network time synchronization failed");
                }
                if (failure_reason & JOIN_FAIL_LINK) {
                    Serial.println("- Link failure: Network link was lost");
                }
            } else if (initiate_network_join()) {
			 
                set_state(STATE_READ_SENSOR);
            }
            break;
        }

        case STATE_READ_SENSOR: {
            read_sensor(&temp, &hum);
            set_state(STATE_SEND_UPLINK);
            break;
        }

        case STATE_SEND_UPLINK: {
            // if uplink is done then go to the next state
            // otherwise keep in idle state
            if (send_uplink(temp, hum))
            {
                set_state(STATE_UPLINK_STATUS);
                last_uplink_time = millis();
            }
            else
            {
                set_state(STATE_IDLE);
            }
            break;
        }

        case STATE_UPLINK_STATUS: {
            // check for the uplink status
            // Check if the uplink is pending or transmitted
            if (mcm.is_last_uplink_pending() && millis() - last_uplink_time < UPLINK_NO_RESPONSE_TIMEOUT_SECONDS * 1000)
            {
                if (currentState != STATE_UPLINK_STATUS)
                {
                    // keep checking the uplink status
                    set_state(STATE_UPLINK_STATUS);
                }
            }
            else
            {
                switch (mcm.get_last_tx_status())
                {
                    case MCM_TX_STATUS::MCM_TX_NOT_SEND:

                        Serial.println("last uplink failed");
                        set_led_state(LED_SENDING_UPLINK_FAIL);
                        break;
                    case MCM_TX_STATUS::MCM_TX_WO_ACK:
                        Serial.println("last uplink sent successfully without ack");
                        break;

                    case MCM_TX_STATUS::MCM_TX_ACK:
                        Serial.println("last uplink sent successfully with ack");
                        break;
                    default:
                        if (false == mcm.is_connected())
                        {
                            Serial.println("MCM is not joined to any network");
                            break;
                        }
                        Serial.println("Uplink failed reason unknown!");
                        break;
                }
                Serial.println();
                set_state(STATE_IDLE);
            }
        }
        break;

        case STATE_NO_LORAWAN_CRED:
            break;

        case STATE_IDLE: {
            // do nothing if ymodem transfer is going on
            if (mcm.ymodem.getState() != YMODEM_IDLE)
            {
                break;
            }
            // Handle downlink if any
            handle_downlink();

            // If MCM has been rebooted, set the connection mode again
            if (mcm.get_context_mgr_is_mcm_reset())
            {
                get_seg_file_status_t file_status;
                // send get segment command
                mcm.get_segmented_file_download_status(&file_status);
                // Serial.println("MCM reset detected, connecting again");
                is_device_joined = false;
                set_led_state(LED_DEVICE_NOT_CONNECTED); // Set LED state for not connected
                set_state(STATE_SET_CONNECT_MODE);
            }

            // Uplink every N seconds
            if (millis() - last_uplink_time > UPLINK_INTERVAL_SECONDS * 1000)
            {
                last_uplink_time = millis();
                set_state(STATE_READ_SENSOR);
            }
            break;
        }
        case STATE_FIRMWARE_UPDATE: {
            Serial.println("Do you want to proceed with firmware update? (y/n)");
            unsigned long start_time = millis();
            bool timeout = false;
            
            while (!Serial.available())
            {
                if (millis() - start_time > 30000)
                { // 30 seconds timeout
                    Serial.println("Timeout waiting for input.");
                    timeout = true;
                    break;
                }
            }

            if (timeout) {
                Serial.println("Firmware update cancelled due to timeout");
                set_state(STATE_SET_CONNECT_MODE);
                break;
            }

            char response = Serial.read();

            // Clear any remaining characters in buffer
            while (Serial.available())
            {
                Serial.read();
            }

            if (response == 'y' || response == 'Y')
            {
                Serial.println("Proceeding with firmware update...");
                mcm.process_fw_update();
                set_state(STATE_IDLE);
                delay(1000);
            }
            else
            {
                Serial.println("Firmware update cancelled");
                set_state(STATE_SET_CONNECT_MODE);
            }

            break;
        }

        default:
            // Handle unexpected states
            break;
    }
}

void send_uplink_now()
{
    currentState = STATE_READ_SENSOR;
}

void send_fw_update_request()
{
    get_seg_file_status_t file_status;
    // send get segment command
    mcm.get_segmented_file_download_status(&file_status);
}

static void handleButtonPress()
{
    if (buttonPressed)
    {
        buttonPressed = false; // Clear the flag

        Serial.println("Button pressed - Switching Network");

        // State machine to cycle through Sidewalk protocols only
        switch (device_mode)
        {
            case ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE:
                device_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK;
                break;
            case ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK:
                device_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS;
                break;
            case ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS:
                device_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE; // Cycle back to BLE
                break;
            default:
                device_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE; // Default to Sidewalk BLE if in an unknown state
                break;
        }

        switch_protocol_mode(device_mode);
    }
}

/**
 * @brief Checks the device connection status and updates the LED state accordingly.
 *
 * This function checks if the device is connected to the network. If the device
 * is connected and has not previously joined, it updates the joined state and
 * sets the appropriate LED state based on the current connection mode. If the
 * device is not connected, it resets the joined state and indicates the disconnection
 * through the LED.
 */
static void check_device_connection()
{
    if (true == mcm.is_connected())
    {
        if (false == is_device_joined)
        {
            is_device_joined = true;
            Serial.println("Device joined successfully");

            switch (device_mode)
            {
                case ConnectionMode::CONNECTION_MODE_LORAWAN:
                    set_led_state(LED_JOINED_LORAWAN_NETWORK);
                    break;
                case ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE:
                    set_led_state(LED_JOINED_SW_BLE_NETWORK);
                    break;
                case ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK:
                    set_led_state(LED_JOINED_SW_FSK_NETWORK);
                    break;
                case ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS:
                    set_led_state(LED_JOINED_SW_CSS_NETWORK);
                    app_SwSetCssPwrProfile(MROVER_CSS_PWR_PROFILE_A);
                    break;
                default:
                    Serial.println("No Connection (NC)");
                    break;
            }
        }
    }
    else
    {
        if (is_device_joined && device_mode != ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE)
        {
            Serial.println("Device not connected to network.");
            set_led_state(LED_DEVICE_NOT_CONNECTED); // Set LED state for not connected
            is_device_joined = false;                // Reset the joined state if not connected
        }
    }
}


int app_queryNextUplink_mtu(uint16_t *mtu)
{
    MCM_STATUS status = mcm.get_next_uplink_mtu(mtu);
    return status != MCM_STATUS::MCM_OK ? -1 : 0;
}

int app_getCachedNextUplink_mtu(uint16_t *mtu) {
    
    if(device_mode == ConnectionMode::CONNECTION_MODE_NC){
        return -1; //early return
    }

    int ret = 0;
    
    static uint16_t preNextUplink_mtu = 0;
    static ConnectionMode preActiveProtocol = ConnectionMode::CONNECTION_MODE_NC;
    if(preActiveProtocol != device_mode || preNextUplink_mtu == 0){
        preActiveProtocol = device_mode;
        do{
            if(mtu == NULL) {
                Serial.println("Error: Null pointer passed to get_next_uplink_mtu");
                ret = -1;
                break;
            }
            memset(mtu, 0, sizeof(uint16_t));

            int status = app_queryNextUplink_mtu(mtu);
            if (status != 0) {
                Serial.println("Error: Failed to get next uplink MTU");
                ret = -1;
                break;
            }

        }while (0);
        preNextUplink_mtu = *mtu;

    }else{
        *mtu = preNextUplink_mtu;
    }
    return ret; // or error code
}

int app_SwSetCssPwrProfile(mrover_css_pwr_profile_t profile) {
  if (profile > MROVER_CSS_PWR_PROFILE_B) {
    return -1;
  }

  if (is_device_joined == false ||
      device_mode != ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS) {
    Serial.println("Current running mode is not sidewalk CCS");
    return -1; // early return
  }

  static mrover_css_pwr_profile_t SW_currentPwrProfile =
      (mrover_css_pwr_profile_t)0xFF;
  if (SW_currentPwrProfile == profile) {
    Serial.printf("Sidewalk CSS Power Profile already set to: %d\n", profile);
    return 0; // No change needed
  }

  // Set the power profile for the Sidewalk CSS connection
  mcm.sw_setCssPwrProfile((uint8_t)profile);
  SW_currentPwrProfile = profile;
  // Print the selected power profile
  char prof_ch[MROVER_CSS_PWR_PROFILE_B + 1] = {'A', 'B'};
  Serial.printf("Sidewalk CSS Power Profile set to: %c\n", prof_ch[profile]);

  return 0;
}