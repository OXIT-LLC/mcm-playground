/**
 * @file frame_parse.h
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief Header file for the frame parser. 
 * @version 0.1
 * @date 2024-04-11
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


#ifndef __FRAME_PARSE_H__
#define __FRAME_PARSE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************
 * INCLUDES
 **********************************************************************************************************/
#include "commands_defs.h"
#include <stdbool.h>

/**********************************************************************************************************
 * MACROS AND DEFINES
 **********************************************************************************************************/

/**********************************************************************************************************
 * TYPEDEFS
 **********************************************************************************************************/
typedef enum {

    FP_SUCCESS = 0,
    FP_INVALID_PARAMETERS,
    FP_INVALID_RETURN_CODE,
    FP_INVALID_COMMAND_TYPE,
    FP_INVALID_COMMAND_CODE,
    FP_INVALID_CRC,
    FP_ERROR

}fp_api_status_t;

/**********************************************************************************************************
 * EXPORTED VARIABLES
 **********************************************************************************************************/

/**********************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************/
/**
 * @brief This function appends the CRC to the data in the buffer.
 *
 * @param[in,out] data Pointer to the buffer containing the data and CRC.
 * @param[in] len Length of the data in the buffer.
 *
 * @return Returns the status of the CRC append operation.
 */
fp_api_status_t fp_append_crc(uint8_t *data,uint16_t len);

/**
 * @brief Check if the frame is valid
 *
 * This function checks if the frame is valid by checking for
 * the return code, command type and command code and the CRC
 *
 * @param[in] data Pointer to the frame data
 * @param[in] len Length of the frame data
 *
 * @retval FP_SUCCESS The frame is valid
 * @retval FP_INVALID_PARAMETERS Invalid data pointer or length
 * @retval FP_INVALID_RETURN_CODE Invalid return code in the frame
 * @retval FP_INVALID_COMMAND_TYPE Invalid command type in the frame
 * @retval FP_INVALID_COMMAND_CODE Invalid command code in the frame
 * @retval FP_INVALID_CRC Invalid CRC in the frame
 * @retval FP_ERROR Generic error
 *
 */
fp_api_status_t fp_is_valid_response_frame(uint8_t *data, uint16_t len);

/**
 * @brief Check if the frame is notification or response
 *
 *
 * @param[in] data Pointer to the frame data
 * @param[in] len Length of the frame data
 *
 * @retval true The frame is notification
 * @retval false The frame is response
 *
 */
bool fp_is_frame_notification(uint8_t *data, uint16_t len);

/**
 * @brief Check if the frame is a valid notify frame
 *
 * This function checks if the frame is valid notify frame by checking for
 * the return code and the notification code.
 *
 * @param[in] data Pointer to the frame data
 * @param[in] len Length of the frame data
 *
 * @retval FP_SUCCESS The frame is valid notify frame
 * @retval FP_INVALID_PARAMETERS Invalid data pointer or length
 * @retval FP_INVALID_RETURN_CODE Invalid return code in the frame
 * @retval FP_INVALID_COMMAND_CODE Invalid command code in the frame
 * @retval FP_ERROR Generic error
 *
 */
fp_api_status_t fp_is_valid_notify_frame(uint8_t *data, uint16_t len);

/**
 * @brief This function returns the number of pending events.
 *
 * @param[in] data Pointer to the notification frame received.
 * @param[in] len Length of the notification frame.
 *
 * @return The number of pending events.
 */
uint8_t fp_get_pending_event_count(uint8_t *data, uint16_t len);

/**
 * @brief Check if the frame is a single frame.
 *
 * This function checks if the frame is a single frame by checking if the first
 * byte is MROVER_RC_NOTIFY_EVENTS and the length is MIN_RX_PAYLOAD_LEN.
 *
 * @param[in] p_data Pointer to the frame data.
 * @param[in] u16_len Length of the frame.
 *
 * @retval true The frame is a single frame.
 * @retval false The frame is not a single frame.
 */
bool fp_is_single_frame(uint8_t *p_data,uint16_t u16_len);



#ifdef __cplusplus
}
#endif




#endif // __FRAME_PARSE_H__