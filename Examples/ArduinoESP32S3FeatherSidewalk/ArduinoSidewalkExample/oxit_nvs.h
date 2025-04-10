/**
 * @file oxit_nvs.h
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief Declarations for non-volatile storage APIs
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

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************
 * INCLUDES
 **********************************************************************************************************/
#include <stdint.h>

/**********************************************************************************************************
 * MACROS AND DEFINES
 **********************************************************************************************************/

/**********************************************************************************************************
 * TYPEDEFS
 **********************************************************************************************************/

/**********************************************************************************************************
 * EXPORTED VARIABLES
 **********************************************************************************************************/

/**********************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************/

/**
 * @brief Initializes the NVS (Non-Volatile Storage) module.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool nvs_storage_init();


/**
 * @brief Retrieves the reboot count from the NVS (Non-Volatile Storage) module.
 *
 * @return The reboot count. If an error occurs, returns 0.
 */
uint16_t nvs_storage_get_reboot_count();

/**
 * @brief Retrieves the LoRaWAN credentials from the NVS (Non-Volatile Storage) module.
 *
 * @param app_key Pointer to the buffer to store the app key.
 * @param join_eui Pointer to the buffer to store the join EUI.
 * @param dev_eui Pointer to the buffer to store the dev EUI.
 *
 * @return true if the credentials are successfully retrieved, false otherwise.
 */
bool nvs_storage_get_lorawan_cred(uint8_t *app_key, uint8_t *join_eui, uint8_t *dev_eui);


/**
 * @brief Stores the LoRaWAN app key in the NVS (Non-Volatile Storage) module.
 *
 * @param app_key Pointer to the buffer containing the app key.
 *
 * @return true if the app key is successfully stored, false otherwise.
 */
bool nvs_storage_set_app_key(uint8_t *app_key);

/**
 * @brief Stores the LoRaWAN join EUI in the NVS (Non-Volatile Storage) module.
 *
 * @param join_eui Pointer to the buffer containing the join EUI.
 *
 * @return true if the join EUI is successfully stored, false otherwise.
 */
bool nvs_storage_set_join_eui(uint8_t *join_eui);

/**
 * @brief Stores the LoRaWAN dev EUI in the NVS (Non-Volatile Storage) module.
 *
 * @param dev_eui Pointer to the buffer containing the dev EUI.
 *
 * @return true if the dev EUI is successfully stored, false otherwise.
 */
bool nvs_storage_set_dev_eui(uint8_t *dev_eui);

/**
 * @brief Erases all data stored in the NVS (Non-Volatile Storage) module.
 *
 * @return true if the erase operation is successful, false otherwise.
 */
bool nvs_storage_erase();

#ifdef __cplusplus
}
#endif