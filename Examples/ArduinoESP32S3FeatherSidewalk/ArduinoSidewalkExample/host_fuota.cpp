/**
 * @file host_fuota.cpp
 * @author Paresh
 * @brief Implementation for host FUOTA APIs
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

#include "host_fuota.h"
#include "api_processor.h"
#include <Arduino.h>

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

/******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
uint32_t get_seg_size_bytes(uint8_t seg_size_type)
{
    uint32_t seg_size_bytes = 0;

    switch (seg_size_type)
    {
        case SEG_SIZE_64:
            seg_size_bytes = FUOTA_SEG_SIZE_BYTES_64;
            break;
        case SEG_SIZE_128:
            seg_size_bytes = FUOTA_SEG_SIZE_BYTES_128;
            break;
        case SEG_SIZE_256:
            seg_size_bytes = FUOTA_SEG_SIZE_BYTES_256;
            break;
        case SEG_SIZE_512:
            seg_size_bytes = FUOTA_SEG_SIZE_BYTES_512;
            break;
        default:
            break;
    }

    return seg_size_bytes;
}

uint32_t calculate_no_of_segments(uint8_t *pkg_size, uint32_t seg_size_byte)
{
   uint32_t pkg_full_size =  pkg_size[0] << 16 | pkg_size[1] << 8 | pkg_size[2];
    return (pkg_full_size + seg_size_byte - 1) / seg_size_byte;
}

bool is_all_segments_downloaded(get_seg_file_status_t seg_file_status)
{
    bool is_downloaded = true;
    uint32_t pkg_full_size =  seg_file_status.pkg_size[0] << 16 | seg_file_status.pkg_size[1] << 8 | seg_file_status.pkg_size[2];   
    // Determine the segment size based on the segment size type
    uint32_t seg_size = get_seg_size_bytes(seg_file_status.seg_size);
    if(seg_size == 0)
    {
        return false;
    }
    uint32_t total_segments = (pkg_full_size + seg_size - 1) / seg_size;
    Serial.printf("Total segments: %d\n", total_segments);
    for (uint32_t i = 0; i < total_segments; i++)
    {
        if ((seg_file_status.seg_status & (1 << i)) != 0)
        {
            is_downloaded = false;
            Serial.printf("Segment %d is not downloaded\n", i);
            break;
        }
    }

    return is_downloaded;
}
uint8_t get_cmd_type_from_seg_file_status(get_seg_file_status_t seg_file_status)
{
    return seg_file_status.cmd_type.cmd_type;
}
