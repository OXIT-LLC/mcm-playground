/**
 * @file mcm_rover.h
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief Header file for the mcm rover 
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


#ifndef __MCM_ROVER_H__
#define __MCM_ROVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************
 * INCLUDES
 **********************************************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include "api_processor.h" 
#include "ymodem.h"
#include "host_fuota.h"

/**********************************************************************************************************
 * MACROS AND DEFINES
 **********************************************************************************************************/

/**
 * @brief size for the buffer 
 * mcm can send upto 306 bytes as response
 * 6 bytes as notification (but adding up 15 bytes for safety)
 * This would needed for y moden protocol
 */
#define BUFFER_SIZE (1036)

#define MCM_ROVER_LIB_VER_MAJOR 0
#define MCM_ROVER_LIB_VER_MINOR 3
#define MCM_ROVER_LIB_VER_PATCH 0

/**********************************************************************************************************
 * TYPEDEFS AND CLASSES
 **********************************************************************************************************/

enum class MCM_STATUS {
    MCM_OK,
    MCM_ERROR,
    MCM_PARAM_ERROR,
    MCM_TIMEOUT
};

enum class ConnectionMode {
    CONNECTION_MODE_NC,
    CONNECTION_MODE_LORAWAN,
    CONNECTION_MODE_SIDEWALK_BLE,
    CONNECTION_MODE_SIDEWALK_FSK,
    CONNECTION_MODE_SIDEWALK_CSS,
};

enum class PayloadType {
    MCM_SW_VER,
    LORAWAN_DEV_EUI,
    LORAWAN_JOIN_EUI,
    LORAWAN_APP_KEY
};

enum class MCM_TX_STATUS {
    MCM_TX_NOT_SEND,
    MCM_TX_WO_ACK,
    MCM_TX_ACK
};

enum class MCM_UPLINK_TYPE{
    MCM_UPLINK_TYPE_NA,
    MCM_UPLINK_TYPE_CONF,
    MCM_UPLINK_TYPE_UNCONF
};

enum class MCM_LORAWAN_CLASS_TYPE
{
    MCM_LRWAN_CLASS_A = 0x00,
    MCM_LRWAN_CLASS_B = 0x01,
    MCM_LRWAN_CLASS_C = 0X02
};

typedef void(*on_rx_callback)(uint8_t *data, uint8_t len,int8_t rssi,uint8_t snr,uint16_t seq_port);


class MCM {

    private:
    
    ConnectionMode current_mode = ConnectionMode::CONNECTION_MODE_NC;
    uint8_t _reset_pin;
    uint32_t _baud_rate;
    uint8_t _rx_pin;
    uint8_t _tx_pin;
    HardwareSerial& __mcm_serial;
    mcm_module_hdl_t *module = NULL;
    uint8_t is_rx_received;
    uint16_t received_size;
    uint8_t sw_reset_evnet_count;
    bool is_joined_network;
    MCM_TX_STATUS last_tx_status;
    bool is_downlink_avail =  false;
    bool is_last_uplink_pend;
    uint8_t downlink_buffer[BUFFER_SIZE];
    uint16_t downlink_buffer_size;
    on_rx_callback on_rx_callback_func = nullptr;
    uint32_t serial_rx_timeout = 2000;
    int8_t rssi;
    uint8_t snr;
    uint16_t seq_port;
    bool is_debug_enabled = false;
    bool _context_mgr_is_joined_cmd_received = false;
    bool _context_mgr_is_mcm_reset;
    void process_received_data();
    
public:
    ver_type_1_t host_version;
    YModem ymodem;
    bool is_new_firmware_downloaded = false;
    get_seg_file_status_t seg_file_status;
    MCM_LORAWAN_CLASS_TYPE _dev_class = MCM_LORAWAN_CLASS_TYPE::MCM_LRWAN_CLASS_A;
    bool _change_class = false;
    MCM(HardwareSerial& serial,uint8_t tx_pin, uint8_t rx_pin, uint8_t reset_pin);
    MCM_STATUS begin();
    String print_version();
    void sw_reset();
    void hw_reset();
    void set_connect_mode(ConnectionMode mode);
    ConnectionMode get_connect_mode();
    MCM_STATUS set_lorawan_credentials(uint8_t *dev_eui, uint8_t *join_eui,uint8_t *app_key);
    MCM_STATUS connect_network();
    void send_uplink(uint8_t *data, uint16_t len,uint8_t port,MCM_UPLINK_TYPE send_uplink);
    void handle_rx_events();
    bool is_connected();
    MCM_TX_STATUS get_last_tx_status();
    bool is_last_uplink_pending();
    void set_on_rx_callback(on_rx_callback callback);
    void stop_network();
    void set_serial_rx_timeout(uint32_t timeout);
    bool is_downlink_available();
    void get_downlink_data(uint8_t *data, uint16_t* len,int8_t* rssi,int8_t* snr,uint16_t* seq_port);
    void set_debug_enabled(bool val);
    MCM_STATUS set_lorawan_class(MCM_LORAWAN_CLASS_TYPE dev_class);
    MCM_LORAWAN_CLASS_TYPE get_lorawan_class();
    MCM_STATUS factory_reset();
    mcm_module_hdl_t* get_module_handle();
    HardwareSerial& get_serial();
    void set_is_rx_received(uint8_t val);
    void set_received_size(uint16_t val);
    uint8_t get_sw_reset_event_count();
    void set_is_joined_network(bool val);
    void set_is_last_uplink_pending(bool val);
    uint8_t* get_downlink_buffer();
    on_rx_callback get_on_rx_callback_func();
    void set_last_tx_status(MCM_TX_STATUS status);
    void set_downlink_meta_data(uint16_t len,int8_t rssi,int8_t snr,uint16_t seq_port,bool val);
    bool get_is_debug_enabled();
    void set_context_mgr_is_joined_cmd_received(bool val);
    bool get_context_mgr_is_joined_cmd_received();
    void set_context_mgr_is_mcm_reset(bool val);
    bool get_context_mgr_is_mcm_reset();
    bool get_context_mgr_is_class_change();
    MCM_STATUS start_file_transfer(ver_type_1_t version);
    MCM_STATUS get_segmented_file_download_status(get_seg_file_status_t *status);
    MCM_STATUS trigger_firmware_update(ver_type_1_t version);
    bool is_new_firmware();
    void set_host_app_version(ver_type_1_t version);
    void retrieveLibraryVersions(ver_type_1_t *mcm_rover_lib_ver, ver_type_1_t *c_lib_ver);
    MCM_STATUS process_fw_update();

};

/**********************************************************************************************************
 * EXPORTED VARIABLES
 **********************************************************************************************************/

/**********************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************/

#ifdef __cplusplus
}
#endif
#endif // __MCM_ROVER_H__