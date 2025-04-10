/**
 * @file oxit_nvs.h
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief Implmentation for non-volatile storage APIs
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
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <Arduino.h>
#include "oxit_nvs.h"
#include "SparkFun_External_EEPROM.h" 
/******************************************************************************
 * USING NAMESPACES
 ******************************************************************************/


/******************************************************************************
 * EXTERN VARIABLES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE MACROS AND DEFINES
 ******************************************************************************/

#define USE_INTERNAL_FLASH 1

#define STORAGE_NAMESPACE "storage"
#define DEVEUI_KEY "deveui"
#define JOIN_EUI_KEY "join_eui"
#define APP_KEY_KEY "app_key"
#define REBOOT_COUNT_KEY "reboot_count" 

#define REBOOT_LOC 0
#define DEVEUI_LOC 8
#define JOIN_EUI_LOC 24
#define APP_KEY_LOC 40

/******************************************************************************
 * PRIVATE TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
ExternalEEPROM myMem;
static bool is_nvs_init = false;
/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************/
bool is_all_ff(uint8_t* arr, uint8_t len);

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
bool nvs_storage_init()
{
#if USE_INTERNAL_FLASH
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    if(ESP_OK != ret)
    {
        return false;
    }

    return true;
#else
    myMem.setMemoryType(512);
    is_nvs_init = myMem.begin();
    return is_nvs_init;
    
#endif 
}

uint16_t nvs_storage_get_reboot_count()
{
    uint16_t reboot_count = 0;
    #if USE_INTERNAL_FLASH
    nvs_handle_t handle;

    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    do
    {
        if (err != ESP_OK) 
        {   
            Serial.println("Failed to open NVS");
            break;
        }

        err = nvs_get_u16(handle, REBOOT_COUNT_KEY, &reboot_count);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND)
        {
            Serial.println("Failed to read reboot count");
            break;
        }

        Serial.println("Updating restart counter in NVS ... ");
        reboot_count = reboot_count + 1;
        err = nvs_set_u16(handle, "reboot_count", reboot_count);
        if (err != ESP_OK)
        {
            Serial.println("Failed to update reboot count");
            break;
        }
        err = nvs_commit(handle);
        if (err != ESP_OK)
        {
            Serial.println("Failed to commit updated reboot count");
            break;
        }
    } while (0);
    
    nvs_close(handle);
#else

    if (false == is_nvs_init)
    {
        return 0;
    }
    myMem.get(0, reboot_count);
    reboot_count = reboot_count + 1;
    myMem.put(0, reboot_count);
#endif
    return reboot_count;
}


bool nvs_storage_get_dev_eui(uint8_t *devui)
{   
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &storage_handle);
    do
    {
        if (err != ESP_OK) 
        {
            Serial.println("Failed to open NVS");
            break;
        }
        size_t required_size = 8;
        err = nvs_get_blob(storage_handle,DEVEUI_KEY, devui, &required_size);
       
        if (err != ESP_OK) 
        {
            Serial.println("Failed to read dev_eui");
            break;
        }
        return_value = true;
    } while (0);
    
    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }

    if (0 == myMem.read(DEVEUI_LOC, devui, 8))
    {
        if(is_all_ff(devui, 8))
        {
            return_value = false;
        }
        else
        {
            return_value = true;
        }
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}

bool nvs_storage_set_dev_eui(uint8_t *devui)
{   
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    do
    {
        if (err != ESP_OK)
        {
            Serial.println("Failed to open NVS");
            break;
        }
        err = nvs_set_blob(storage_handle, DEVEUI_KEY, devui, 8);
        if (err != ESP_OK)
        {
            Serial.println("Failed to write dev_eui");
            break;
        }
       
        err = nvs_commit(storage_handle);
        if (err != ESP_OK)
        {
            Serial.println("Failed to commit updated dev_eui");
            break;
        }
        return_value = true;
    } while (0);

    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }

    if (0 == myMem.write(DEVEUI_LOC, devui, 8))
    {
        return_value = true;
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}

bool nvs_storage_get_join_eui(uint8_t *join_eui)
{
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &storage_handle);
    do
    {
        if (err != ESP_OK)
        {
            Serial.println("Failed to open NVS");
            break;
        }
        size_t required_size = 8;
        err = nvs_get_blob(storage_handle, JOIN_EUI_KEY, join_eui, &required_size);
        
        if (err != ESP_OK)
        {
            Serial.println("Failed to read join_eui");
            break;
        }
        return_value = true;
    } while (0);

    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }
    if (0 == myMem.read(JOIN_EUI_LOC, join_eui, 8))
    {
         if(is_all_ff(join_eui, 8))
        {
            return_value = false;
        }
        else
        {
            return_value = true;
        }
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}


bool nvs_storage_set_join_eui(uint8_t *join_eui)
{
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err; 
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    do
    {
        if (err != ESP_OK)
        {
            Serial.println("Failed to open NVS");
            break;  
        }
        err = nvs_set_blob(storage_handle, JOIN_EUI_KEY, join_eui, 8);
        if (err != ESP_OK)
        {
            Serial.println("Failed to write join_eui");
            break;
        }
       
        err = nvs_commit(storage_handle);
        if (err != ESP_OK)
        {
            Serial.println("Failed to commit updated join_eui");
            break;
        }
        return_value = true;
    } while (0);
    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }

    if (0 == myMem.write(JOIN_EUI_LOC, join_eui, 8))
    {
        return_value = true;
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}   

bool nvs_storage_get_app_key(uint8_t *app_key)
{
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &storage_handle);
    do
    {
        if (err != ESP_OK)
        {
            Serial.println("Failed to open NVS");
            break;
        }
        size_t required_size = 16;
        err = nvs_get_blob(storage_handle,APP_KEY_KEY, app_key, &required_size);
        
        if (err != ESP_OK)
        {
            Serial.println("Failed to read app_key");
            break;
        }
        return_value = true;
    } while (0);

    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }
    if (0 == myMem.read(APP_KEY_LOC, app_key, 16))
    {   
        if(is_all_ff(app_key, 16))
        {
            return_value = false;
        }
        else
        {
            return_value = true;
        }
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}

bool nvs_storage_set_app_key(uint8_t *app_key)
{
    bool return_value = false;
#if USE_INTERNAL_FLASH
    nvs_handle_t storage_handle;
    esp_err_t err;  
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);  
    do
    {           
        if (err != ESP_OK)
        {   
            Serial.println("Failed to open NVS");
            break;                  
        }
        err = nvs_set_blob(storage_handle, APP_KEY_KEY, app_key, 16); 
        if (err != ESP_OK)
        {   
            Serial.println("Failed to write app_key");
            break;                  
        }
        
        err = nvs_commit(storage_handle);
        if (err != ESP_OK)
        {   
            Serial.println("Failed to commit updated app_key");
            break;                  
        }
        return_value = true;    
    } while (0);
    nvs_close(storage_handle);
#else
    if(false == is_nvs_init)
    {
        return false;
    }

    if (0 == myMem.write(APP_KEY_LOC, app_key, 16))
    {
        return_value = true;
    }
    else
    {
        return_value = false;
    }
#endif
    return return_value;
}

bool nvs_storage_erase()
{
#if USE_INTERNAL_FLASH
    esp_err_t err = nvs_flash_erase();
    if (err == ESP_OK)
    {
        return true;
    }
    return false;
#else
    myMem.erase(0xFF);
    return true;
#endif
}

bool nvs_storage_get_lorawan_cred(uint8_t *app_key, uint8_t *join_eui, uint8_t *dev_eui)
{
    bool return_value = false;
    do
    {
        if (false ==nvs_storage_get_app_key(app_key))
        {
            Serial.println("Failed to read app_key from nvs_storage");
            break;
        }
        if (false ==nvs_storage_get_join_eui(join_eui))
        {
            Serial.println("Failed to read join_eui from nvs_storage");
            break;
        }
        if (false ==nvs_storage_get_dev_eui(dev_eui))
        {
            Serial.println("Failed to read dev_eui from nvs_storage");
            break;
        }
        return_value = true;
    } while (0);
    
    return return_value;

}



/******************************************************************************
 * END OF FILE
 ******************************************************************************/

