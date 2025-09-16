/**
 * @file frame_parser.c
 * @author Ankit Bansal (ankit.bansal@gmail.com)
 * @brief this contain the implementation of the frame parser. This file 
 * contains definition for the apis to create and parse frames, as required by 
 * the api_process
 * @version 0.1
 * @date 2024-04-15
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
#include "frame_parse.h"
#include <stdbool.h>

/******************************************************************************
 * EXTERN VARIABLES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE MACROS AND DEFINES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/


/**
 * @brief This function calculates the CRC of the given data
 * 
 * @param[in] u8_data Pointer to the data for which CRC needs to be calculated
 * @param[in] u16_len Length of the data
 * @return The calculated CRC of the data
 */
static uint8_t fp_calculate_crc(uint8_t *u8_data, uint16_t u16_len)
{
    uint8_t u8_crc = 0;
    for (uint16_t u16_index = 0; u16_index < u16_len; u16_index++)
    {
        u8_crc ^= u8_data[u16_index];
    }

    return u8_crc;
}

/**
 * @brief This function checks if the received frame is a valid response frame
 * 
 * @param[in] u8_response_code Response code of the frame received
 * @return true if the response code is a valid one, false otherwise
 */
static bool is_valid_response_code(uint8_t u8_response_code)
{
    bool b_is_valid_repose_code = false;

    switch (u8_response_code)
    {   
        // intentional fall through for the cases
        case MROVER_RC_OK:
        case MROVER_RC_UNKNOWN:
        case MROVER_RC_NOT_IMPLEMENTED:
        case MROVER_RC_FAIL:
        case MROVER_RC_BAD_CRC:
        case MROVER_RC_BAD_SIZE:
        case MROVER_RC_NOTIFY_EVENTS:
        case MROVER_RC_GPS_TIME_NOT_AVAILABLE:
            b_is_valid_repose_code = true;
        break;

        default:
            break;
    }
    return b_is_valid_repose_code;
}

/**
 * @brief This function checks if the received command type is valid
 * 
 * @param[in] u8_command_type Command type received
 * @return true if the command type is valid, false otherwise
 */
static bool is_valid_command_type(uint8_t u8_command_type)
{
    bool b_is_valid_command_type = false;

    switch (u8_command_type)
    {
            // intentional fall through for the cases
        case COMMAND_TYPE_GENERAL:
        case COMMAND_TYPE_LORAWAN:
        case COMMAND_TYPE_SIDEWALK:
            b_is_valid_command_type = true;
            break;

        default:
            break;
    }

    return b_is_valid_command_type;
}

/**
 * @brief This function checks if the received command code is valid
 * 
 * @param[in] u16_command_code Command code received
 * @return true if the command code is valid, false otherwise
 */
static bool is_valid_command_code(uint16_t u16_command_code)
{
    bool b_is_valid_command_code = false;

    switch (u16_command_code)
    {
        // intentional fall through for the cases
    case MROVER_CC_GET_EVENT:
    case MROVER_CC_GET_VERSION:
    case MROVER_CC_RESET:
    case MROVER_CC_FACTORY_RESET:
    case MROVER_CC_SWITCH_NETWORK:
    case MROVER_CC_INIT_LORAWAN:
    case MROVER_CC_SET_JOIN_EUI:
    case MROVER_CC_GET_GPS_TIME:
    case MROVER_CC_LORAWAN_TIME_REQ:
    case MROVER_CC_SET_DEV_EUI:
    case MROVER_CC_SET_NW_KEY:
    case MROVER_CC_GET_DEV_EUI:
    case MROVER_CC_GET_JOIN_EUI:
    case MROVER_CC_JOIN_LORAWAN:
    case MROVER_CC_REQUEST_UPLINK:
    case MROVER_CC_LEAVE_LORAWAN_NETWORK:
    case MROVER_CC_STOP_SID_LORAWAN_NETWORK:
    case MROVER_CC_BLE_LINK_REQUEST:
    case MROVER_CC_BLE_CONNECTION_REQUEST:
    case MROVER_CC_FSK_LINK_REQUEST:
    case MROVER_CC_CSS_LINK_REQUEST:
    case MROVER_CC_SET_CSS_PWR_PROFILE:
    case MROVER_CC_SET_FILTERING_DOWNLINK_SIDEWALK:
    case MROVER_CC_GET_LORAWAN_CLASS:
    case MROVER_CC_SET_LORAWAN_CLASS:
    case MROVER_CC_START_FILE_TRANSFER:
    case MROVER_CC_FILE_STATUS:
    case MROVER_CC_TRIGGER_FW_UPDATE:
    case MROVER_CC_GET_LAST_DL_STATS:
    case MROVER_CC_GET_NEXT_UPLINK_MTU:
    case MROVER_CC_SELFTEST_TRIGGER:
    case MROVER_CC_SELFTEST_RESULT_QUERY:
        b_is_valid_command_code = true;
        
        break;
    
    default:
        break;
    }

    return b_is_valid_command_code;

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



fp_api_status_t fp_append_crc(uint8_t *data,uint16_t len)
{   
    fp_api_status_t return_status = FP_ERROR;

    do
    {
        if(NULL == data)
        {
            TRACE_INFO("Data pointer cannot be NULL\n");
            return_status = FP_INVALID_PARAMETERS;
            break;
        }

        if(MIN_TX_PAYLOAD_LEN > len)
        {
            TRACE_INFO("Minimum payload length is %d\n",MIN_TX_PAYLOAD_LEN);
            return_status = FP_INVALID_PARAMETERS;
            break;
        }

        data[len - 1] = fp_calculate_crc(data, len);
        return_status = FP_SUCCESS;
    } while (0);
    
    return return_status;

}


fp_api_status_t fp_is_valid_response_frame(uint8_t *data, uint16_t len)
{
    fp_api_status_t return_status = FP_ERROR;

    do
    {
        if (false == is_valid_response_code(data[0]))
        {
            TRACE_INFO("Invalid response code %d\n", data[0]);
            return_status = FP_INVALID_RETURN_CODE;
            break;
        }

        if (false == is_valid_command_type(data[1]))
        {
            TRACE_INFO("Invalid command code %d\n", data[1]);
            return_status = FP_INVALID_COMMAND_TYPE;
            break;
        }

        uint16_t u16_command_code = (data[2] << 8) | data[3];
        if (false == is_valid_command_code(u16_command_code))
        {
            TRACE_INFO("Invalid command code %d\n", u16_command_code);
            return_status = FP_INVALID_COMMAND_TYPE;
            break;
        }

        // check for crc

        uint8_t crc = fp_calculate_crc(data, len - 1);
        if (crc != data[len - 1])
        {
            TRACE_INFO("Invalid CRC\n");
            return_status = FP_INVALID_CRC;
            break;
        }

        return_status = FP_SUCCESS;

    } while (0);

    return return_status;
}


bool fp_is_frame_notification(uint8_t *data, uint16_t len)
{
   return (MROVER_RC_NOTIFY_EVENTS == data[0]?true:false);
}


fp_api_status_t fp_is_valid_notify_frame(uint8_t *data, uint16_t len)
{
    fp_api_status_t return_status = FP_ERROR;

    do
    {   
        // validate crc
        uint8_t crc = fp_calculate_crc(data, len - 1);
        if (crc != data[len - 1])
        {
            TRACE_INFO("Invalid CRC for the notification frame\n");
            return_status = FP_INVALID_CRC;
            break;
        }

        uint16_t u16_len = (data[1] << 8) | data[2];

        if(LENGTH_IN_NOTIFICATION_PAYLOAD != u16_len)
        {
            TRACE_INFO("Invalid length for notification %d\n", u16_len);
            break;
        }

        uint8_t u8_pending_messages = data[3];
        if(MAX_PENDING_MESSAGES < u8_pending_messages)
        {
            TRACE_INFO("Invalid pending messages %d\n", u8_pending_messages);
            break;
        }

        return_status = FP_SUCCESS;
    }while(0);

    return return_status;
}


uint8_t fp_get_pending_event_count(uint8_t *data, uint16_t len)
{
    return data[3];
}


bool fp_is_single_frame(uint8_t *p_data,uint16_t u16_len)
{   
    bool b_return_value = false;
    // check if first byte is MROVER_RC_NOTIFY_EVENTS
    if(MROVER_RC_NOTIFY_EVENTS == p_data[0])
    {
        if(MIN_RX_PAYLOAD_LEN < u16_len) // as the notification frame is always MIN_RX_PAYLOAD_LEN
        {
            b_return_value = false;
        }
        else
        {
            b_return_value = true;
        }
    }

    else
    {
        // decode for the response frame
        uint16_t u16_command_len = (p_data[4] << 8) | p_data[5];
        // 1 byte return code, 1 byte command code 
        // 2 byte command code 2 byte length 
        // u16_command_len payload length
        // 1 byte crc
        uint16_t rec_total_len = 1 + 1 + 2 + 2 + u16_command_len + 1;
        if(rec_total_len == u16_len)
        {
            b_return_value = true; // single frame
        }
        else
        {
            b_return_value = false; // multiple frame
        }
    }

    return b_return_value;
}