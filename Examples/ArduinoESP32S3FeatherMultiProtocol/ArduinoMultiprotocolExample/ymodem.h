/**
 * @file ymodem.h
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief Header file for YModem protocol implementation
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

#ifndef __YMODEM_H__
#define __YMODEM_H__

/**********************************************************************************************************
 * INCLUDES
 **********************************************************************************************************/
#include <stdint.h>
#include <Arduino.h>

/**********************************************************************************************************
 * MACROS AND DEFINES
 **********************************************************************************************************/

#define SOH 0x01   // Start of 128-byte data packet
#define STX 0x02   // Start of 1024-byte data packet
#define EOT 0x04   // End of transmission
#define ACK 0x06   // Acknowledge
#define NAK 0x15   // Negative Acknowledge
#define CRC16 0x43 // 'C' byte to request CRC16

#define YMODEM_TIMEOUT (30*1000)

/**********************************************************************************************************
 * TYPEDEFS
 **********************************************************************************************************/

typedef enum
{
    YMODEM_IDLE,
    WAIT_FOR_HEADER,
    RECEIVE_CHUNK,
    RECEIVE_DATA,
    WAIT_EOT,
    UPDATE_ESP32,
    COMPLETE
} ymodem_state_t;



class YModem
{
public:
    // Add constructor to initialize the reference
    YModem(HardwareSerial& serial) : __ymodem_serial(serial) {}

    void setState(ymodem_state_t state);

    void receivePacket(uint8_t *buffer, uint16_t &size);
    ymodem_state_t getState();
    void sendCRCRequest();
    bool update_esp32_firmware();
    void process_timeout();
    
private:
    ymodem_state_t _state = YMODEM_IDLE;
    HardwareSerial& __ymodem_serial;
    uint64_t _timeout;
    void sendACK();
    void sendNAK();

    // Calculates the CRC for a given data buffer
    uint16_t calculateCRC(const uint8_t *data, uint16_t length);
};

/**********************************************************************************************************
 * EXPORTED VARIABLES
 **********************************************************************************************************/

/**********************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************/

#endif // __YMODEM_H__