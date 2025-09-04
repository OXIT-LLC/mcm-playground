/**
 * @file    command_line_application.c
 * @author  OXIT embedded firmware team
 * @brief   This source file defines the command line application APIs
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

/******************************************************************************/
/* Include Files */
/******************************************************************************/
/**<sys includes*/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

/**<Application includes*/
#include "hal_error_codes.h"
#include "oxit_cli.h"
#include "oxit_cli_app.h"
#include <oxit_nvs.h>
#include "ArduinoMultiprotocolExample.h"


#define CLI_APP_NAME "oxit_cli"
/******************************************************************************/
/* Scope Limited variables */
/******************************************************************************/
static char cli_buffer[128] = {0};

static bool disable_cli_input = false;

/******************************************************************************/
/* Static Function Declaration */
/******************************************************************************/
/**
 * @brief Sets the device EUI.
 *
 * @param pu8_input_value The input value for device EUI.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int set_deveui_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Sets the join EUI.
 *
 * @param pu8_input_value The input value for join EUI.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int set_join_eui_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Sets the application key.
 *
 * @param pu8_input_value The input value for application key.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int set_app_key_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Erases the stored credentials.
 *
 * @param pu8_input_value The input value.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int erase_credentials_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Reboots the device.
 *
 * @param pu8_input_value The input value.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int reboot_device_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Sends an uplink message immediately.
 *
 * @param pu8_input_value The input value.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int send_uplink_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Switches the protocol mode.
 *
 * @param pu8_input_value The input value.
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int protocol_switch_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Sends the fw update request.
 *
 * @param pu8_input_value The input value.
 * @param pfun_uart_tx A function to send bytes over uart.
 *
 * @return int Return value.
 *
 * @throws None
 */
static int send_fw_update_request_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Retrieves the current GPS time in Unix timestamp format.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx A function to send bytes over UART to request GPS time.
 *
 * @return int Return value indicating success (0) or failure (non-zero).
 */
static int get_gps_time_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Synchronizes the device time with the LoRaWAN network.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int lorawan_time_sync_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Retrieves the last downlink statistics.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int get_dl_stats_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief cli_send_bytes call back to send the bytes
 *
 * @return true   If receive data not available or error occured while
 * processing
 * @return false  If success
 */
hal_error_code_t cli_send_bytes(const uint8_t *p_u8_tx_bytes, size_t len);
/**
 * @brief cli_receive_byte call back to receive the bytes
 *
 * @return true   If receive data not available or error occured while
 * processing
 * @return false  If success
 */
hal_error_code_t cli_receive_byte(uint8_t *p_u8_rx_byte);

void helper_print_hex_array(const uint8_t* arr, size_t len);

/**
 * @brief Get the next uplink mtu callback object
 * 
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx  Function to send bytes over UART.
 * @return int Return status code.
 */
static int get_next_uplink_mtu_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Set the CSS power profile callback
 * 
 * @param pu8_input_value input value (profile value)
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int sw_set_css_pwr_profile_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);

/**
 * @brief Triggers a self-test on the MCM device.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int selftestTriggerCallback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx);
/******************************************************************************/
/* Global Variable Definition */
/******************************************************************************/
cli_config_t cli_cmds_cfg = {
     '\r',
     " ",
     "?",
     STARTUP_MSG,
     true,
     false,

};

cli_handle_t cli_hdl = {
    cli_buffer,
    128,
    &cli_cmds_cfg,
    cli_receive_byte,
    cli_send_bytes,
    false,
    0,
    { NULL,NULL},

};
//this function definition is in the main ino file
void send_uplink_now();

void send_fw_update_request();

/******************************************************************************/
/* enter_bootloader application variable */
/******************************************************************************/
cli_command_t cli_commands[] = {{
                                                "deveui",
                                                CLI_APP_NAME" deveui 001122334455667788 <enter>",
                                                "To set lorawan deveui",
                                                set_deveui_callback,
                                            },
                                            {
                                                 "joineui",
                                                 CLI_APP_NAME" joineui 001122334455667788 <enter>",
                                                 "To set lorawan joineui",
                                                 set_join_eui_callback,
                                            },
                                            {
                                                 "appkey",
                                                 CLI_APP_NAME" appkey 00112233445566778899aabbccddeeff <enter>",
                                                 "To set lorawan appkey",
                                                 set_app_key_callback,
                                            },
                                            {
                                                 "reboot",
                                                 CLI_APP_NAME" reboot <enter>",
                                                 "To reboot the device",
                                                 reboot_device_callback,
                                            },
                                            {
                                                 "erase",
                                                 CLI_APP_NAME" erase <enter>",
                                                 "To erase stored credentials",
                                                 erase_credentials_callback,
                                            },
                                            {
                                                 "uplink_now",
                                                 CLI_APP_NAME" send_uplink <enter>",
                                                 "To send uplink",
                                                 send_uplink_callback,
                                            },
                                            {
                                                "fw_update",
                                                CLI_APP_NAME" Send the firmware update request <enter>",
                                                "To send the firmware update request",
                                                send_fw_update_request_callback,
                                            },
                                            {
                                                "protocol_switch",
                                                CLI_APP_NAME" protocol_switch <mode>",
                                                "Switch protocol mode. Modes: lorawan, sw_ble, sw_fsk, sw_css",
                                                protocol_switch_callback,
                                            },
                                            {
                                                "get_gps_time",
                                                CLI_APP_NAME" get_gps_time <enter>",
                                                "To get current GPS timestamp",
                                                get_gps_time_callback,
                                            },
                                            {
                                                "lorawan_time_sync",
                                                CLI_APP_NAME" lorawan_time_sync <enter>",
                                                "Synchronize time with the LoRaWAN network",
                                                lorawan_time_sync_callback,
                                            },
                                            {
                                                "get_dl_stats",
                                                CLI_APP_NAME" get_dl_stats <enter>",
                                                "Retrieve the last downlink statistics",
                                                get_dl_stats_callback,
                                            },
                                            {
                                                "get_next_uplink_mtu",
                                                CLI_APP_NAME" get_next_uplink_mtu <enter>",
                                                "To get next uplink MTU size",
                                                get_next_uplink_mtu_callback,
                                            },
                                            {
                                                "sidewalk_css_prof_switch",
                                                CLI_APP_NAME" sidewalk_css_prof_switch <profile>",
                                                "To set the sidewalk css profile",
                                                sw_set_css_pwr_profile_callback,
                                            },
                                            {
                                                "self_test",
                                                CLI_APP_NAME" self_test <enter>",
                                                "To trigger self test on the MCM",
                                                selftestTriggerCallback,
                                            },
                                            };

cli_app_t register_app = {  CLI_APP_NAME, 
                                        "1.0.0",
                                        cli_commands, 
                                        sizeof(cli_commands)/sizeof(cli_command_t), 
                                        };

/******************************************************************************/
/* Function Definition */
/******************************************************************************/
hal_error_code_t cli_receive_byte(uint8_t *pu8_rx_byte)
{
    static uint8_t u8_byte   = 0;
    hal_error_code_t enu_err = HAL_ERROR_OUT_OF_BOUND;


    u8_byte = Serial.read();

    if ((u8_byte >= 0) && (u8_byte < 255))
    {
        *pu8_rx_byte = u8_byte;
        enu_err      = HAL_ERROR_OK;
    }
    return enu_err;
}
hal_error_code_t cli_send_bytes(const uint8_t *p_u8_tx_bytes, size_t len)
{
    char u8_print_char = 0;

    for (uint32_t u32_index = 0; u32_index <len; u32_index++)
    {
        u8_print_char = (char )*p_u8_tx_bytes;
        if (u8_print_char == CR)
        {
            Serial.print(LF);
        }
        Serial.print(u8_print_char);

        p_u8_tx_bytes++;
    }
    return HAL_ERROR_OK;
}
#pragma optimize("", off)
void convert_string_hex(const uint8_t *s_buf, uint8_t s_len, uint8_t *h_buf, uint8_t h_len)
{   
    // helper_print_hex_array(s_buf, s_len);
     char s_val[2] = {0};
     char *endptr = NULL;
     volatile uint8_t converted = 0;
    for(uint8_t i = 0; i < h_len;i++)
    {   
        strncpy(s_val, (const char*)&s_buf[2 * i], 2);
        
        // if (sscanf(s_val, "%02x", &converted) != 1)
        // {   
        //     Serial.println("Error converting string to hex");
        //     break;
        // }
        h_buf[i] = strtol(s_val, &endptr, 16);

        
    }
//    helper_print_hex_array(h_buf, h_len);
   Serial.println("");
}   
#pragma optimize("", on)

#pragma optimize("", off)
bool validate_hex_conversion(const uint8_t *buf, uint8_t len)
{
    bool return_value = true;
    for(uint8_t i = 0; i < len; i++)
    {
        if(!isxdigit(buf[i]))
        {
            return_value = false;
            break;
        }
    }
    return return_value;
}
#pragma optimize("", on)

#pragma optimize("", off)
static int set_deveui_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    //perform the deveui validation
    do
    {  
        // Serial.println("Received data len : " + String(strlen(pu8_input_value)));
        if(strlen(pu8_input_value) != 16)
        {
            Serial.println("Invalid deveui length, Please try again");
            break;
        }

        if(validate_hex_conversion((const uint8_t*)pu8_input_value, 16) == false)
        {
            Serial.println("Invalid deveui hex value, Please try again");
            break;
        }
        uint8_t deveui[8];
       memset(deveui, 0, sizeof(deveui));
        // convert string to hex
        convert_string_hex((const uint8_t*)pu8_input_value, 16, deveui, sizeof(deveui));

        volatile bool ret = nvs_storage_set_dev_eui(deveui);

        if(false == ret)
        {
            Serial.println("Failed to store deveui, Please try again");
            break;
        }
        // helper_print_hex_array(deveui, sizeof(deveui));  
        Serial.println("Deveui set successfully");
        Serial.println("Please reboot the device manually if all credentials dev eui and join eui and app key are set successfully");

    } while (0);

    return 1;
}
#pragma optimize("", on)

#pragma optimize("", off)
static int set_join_eui_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
     //perform the deveui validation
    do
    {  
        if(strlen(pu8_input_value) != 16)
        {
            Serial.println("Invalid join eui length, Please try again");
            break;
        }

        if(validate_hex_conversion((const uint8_t*)pu8_input_value, 16) == false)
        {
            Serial.println("Invalid join eui hex value, Please try again");
            break;
        }
         uint8_t joineui[8];
       memset(joineui, 0, sizeof(joineui));
        // convert string to hex
        convert_string_hex((const uint8_t*)pu8_input_value, 16, joineui, sizeof(joineui));

        if(false == nvs_storage_set_join_eui(joineui))
        {
            Serial.println("Failed to store join eui, Please try again");
            break;
        }
       
        
        Serial.println("joineui set successfully");
        Serial.println("Please reboot the device manually if all credentials dev eui and join eui and app key are set successfully");

    } while (0);

    return 1;
}
#pragma optimize("", on)

#pragma optimize("", off)
static int set_app_key_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
     //perform the deveui validation
    do
    {  
        if(strlen(pu8_input_value) != 32)
        {
            Serial.println("Invalid app key length, Please try again");
            break;
        }

        if(validate_hex_conversion((const uint8_t*)pu8_input_value, 32) == false)
        {
            Serial.println("Invalid appkey hex value, Please try again");
            break;
        }
        uint8_t app_key[16];
       memset(app_key, 0, sizeof(app_key));
        // convert string to hex
        convert_string_hex((const uint8_t*)pu8_input_value, 32, app_key, sizeof(app_key));

        if(false == nvs_storage_set_app_key(app_key))
        {
            Serial.println("Failed to store app key, Please try again");
            break;
        }
       
       
        Serial.println("app key set successfully");
        Serial.println("Please reboot the device manually if all credentials dev eui and join eui and app key are set successfully");

    } while (0);

    return 1;
}
#pragma optimize("", on)

/**
 * @brief Initializes the command line interface by providing application
 *        specific commands list, maximum number of commands available,
 *        rx callback function and tx callback function.
 *
 */
void init_command_line_app(void)
{
    hal_error_code_t enu_error = HAL_ERROR_OK;

    do
    {
        enu_error = cli_init(&cli_hdl, &cli_cmds_cfg);
        
        enu_error = cli_register_new_app(&cli_hdl, &register_app);

    } while (0);
}

/**
 * @brief  Wraps the comand line process function from the CLI module
 *
 * @return true   If receive data not available or error occured while
 * processing
 * @return false  If success
 */
bool process_command_line_app(void)
{
    if (false == disable_cli_input)
    {
        return cli_process(&cli_hdl);
    }
    else
    {
        return false;
    }
}

static int erase_credentials_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{   
    if(false == nvs_storage_erase())
    {
        Serial.println("Failed to erase credentials, Please try again");
        return 1;
    }
    Serial.println("Credentials erased successfully, Please reboot device and then enter credentials manually");
    return 1;
}

static int reboot_device_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    Serial.println("Rebooting device in 3 seconds");
    delay(3000);
    ESP.restart();
    return 1;
}

static int send_uplink_callback(const char *pu8_input_value,cli_send_bytes_t pfun_uart_tx)
{
    send_uplink_now();
    return 1;
}

static int send_fw_update_request_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    send_fw_update_request();
    return 1;
}

static int protocol_switch_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    // Check if user supplied a mode string
    if (pu8_input_value == NULL || strlen(pu8_input_value) == 0)
    {
        Serial.println("Usage: protocol_switch <mode>");
        Serial.println("Available modes: lorawan, sidewalk_ble, sidewalk_fsk, sidewalk_css");
        return 1;
    }

    // Copy and convert the parameter to lower-case for simple comparison
    char mode_param[32] = {0};
    strncpy(mode_param, pu8_input_value, sizeof(mode_param) - 1);
    for (int i = 0; mode_param[i]; i++)
    {
        mode_param[i] = tolower(mode_param[i]);
    }

    // Parse the parameter and prepare the corresponding ConnectionMode value
    ConnectionMode new_mode;
    if (strcmp(mode_param, "lorawan") == 0)
    {
        new_mode = ConnectionMode::CONNECTION_MODE_LORAWAN;
    }
    else if (strcmp(mode_param, "sw_ble") == 0)
    {
        new_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_BLE;
    }
    else if (strcmp(mode_param, "sw_fsk") == 0)
    {
        new_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_FSK;
    }
    else if (strcmp(mode_param, "sw_css") == 0)
    {
        new_mode = ConnectionMode::CONNECTION_MODE_SIDEWALK_CSS;
    }
    else
    {
        Serial.println("Invalid protocol mode specified.");
        Serial.println("Available modes: lorawan, sw_ble, sw_fsk, sw_css");
        return 1;
    }

    // Call the new protocol switch API to update the mode
    switch_protocol_mode(new_mode);
    return 1;
}


static int get_gps_time_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    // Call the GPS time retrieval function
    uint32_t gps_time = 0;
    int status = get_gps_timestamp(&gps_time);
    
    if (status != 0) {
        Serial.println("Error: Failed to get GPS time");
        return 1;
    }
    
    // Print the GPS time in Unix timestamp format
    Serial.print("Current GPS time: ");
    Serial.println(gps_time);
    
    return 0;
}

/**
 * @brief Synchronizes the device time with the LoRaWAN network.
 *
 * This function requests time synchronization with the LoRaWAN network and provides
 * detailed feedback about the operation's success or failure.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code: 0 on success, 1 on failure
 */
static int lorawan_time_sync_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    Serial.println("Requesting LoRaWAN time synchronization...");
    
    int status = request_lorawan_time_sync();
    if (status != 0) {
        Serial.println("Error: Failed to synchronize time with LoRaWAN network");
        return 1;
    }
    
    return 0;
}

/**
 * @brief Retrieves the last downlink statistics.
 *
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx Function to send bytes over UART.
 * @return int Return status code.
 */
static int get_dl_stats_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    get_last_dl_stats_t dl_stats;
    int status = app_get_dl_stats(&dl_stats);
    
    if (status != 0) {
        Serial.println("Error: Failed to retrieve downlink statistics");
        return 1;
    }

    // Print the detailed downlink statistics
    Serial.printf("Last Downlink Statistics:\n"
                  "\tProtocol Type: %d\n"
                  "\tRSSI: %d\n"
                  "\tSNR: %d\n"
                  "\tTimestamp: %lu\n",
                  dl_stats.protocol_type, dl_stats.rssi, dl_stats.snr, dl_stats.timestamp);
    
    return 0;
}

/**
 * @brief Get the next uplink mtu callback object
 * 
 * @param pu8_input_value The input value (unused for this command).
 * @param pfun_uart_tx  Function to send bytes over UART.
 * @return int Return status code.
 */
static int get_next_uplink_mtu_callback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx)
{
    uint16_t mtu = 0;
    
    int status = app_queryNextUplink_mtu(&mtu);
    if (status != 0) {
        Serial.println("Error: Failed to get next uplink MTU");
        return 1;
    }
    Serial.print("Next Uplink MTU: ");
    Serial.println(mtu);
    return 0;
}

static int sw_set_css_pwr_profile_callback(const char *pu8_input_value,
                                        cli_send_bytes_t pfun_uart_tx) {
  // Check if user supplied a mode string
  uint8_t len = strlen(pu8_input_value);
  if (pu8_input_value == NULL || len == 0 || len > 7) {
    Serial.println("Usage: sidewalk_css_prof_switch <profile>");
    Serial.println("Available modes: prof_a and prof_b");
    return 1;
  }

  // Copy and convert the parameter to lower-case for simple comparison
  char mode_param[7] = {0};
  strncpy(mode_param, pu8_input_value, len);
  for (int i = 0; i < len; i++) {
    mode_param[i] = tolower(mode_param[i]);
  }

  // Parse the parameter and prepare the corresponding ConnectionMode value
  mrover_css_pwr_profile_t prof = (mrover_css_pwr_profile_t)0xFF;
  if (strcmp(mode_param, "prof_a") == 0) {
    prof = MROVER_CSS_PWR_PROFILE_A;
  } else if (strcmp(mode_param, "prof_b") == 0) {
    prof = MROVER_CSS_PWR_PROFILE_B;
  } else {
    Serial.println("Invalid protocol profile specified.");
    Serial.println("Available modes: prof_a, prof_b");
    return 1;
  }
  // Call the new protocol switch API to update the mode
  app_SwSetCssPwrProfile(prof);

  return 0;
}

static int selftestTriggerCallback(const char *pu8_input_value, cli_send_bytes_t pfun_uart_tx) {
    // Call the self-test function
    int status = app_triggerSelfTest();
    if (status != 0) {
        Serial.println("Error: Failed to trigger self-test");
        return 1;
    }
    return 0;
}