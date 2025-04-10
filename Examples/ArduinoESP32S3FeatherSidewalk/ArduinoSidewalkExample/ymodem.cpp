#include <stdint.h>
/**
 * @file ymodem.cpp
 * @author Ankit Bansal (ankit.bansal@oxit.com)
 * @brief
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

#include "ymodem.h"
#include <SPIFFS.h>
#include <Update.h>

/******************************************************************************
 * EXTERN VARIABLES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE MACROS AND DEFINES
 ******************************************************************************/
#define FUOTA_FILE_NAME "/fota.bin"
/******************************************************************************
 * PRIVATE TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
uint8_t ymodem_buffer[1030] = {0};
uint16_t ymodem_buffer_size = 0;
/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/
bool YModem::update_esp32_firmware()
{
    Serial.printf("[YMODEM FW] Opening firmware file...\n");
    File file = SPIFFS.open(FUOTA_FILE_NAME, "r");

    if (!file)
    {
        Serial.printf("[YMODEM FW] ERR: Firmware file open failed.\n");
        return false;
    }

    size_t fileSize = file.size();
    Serial.printf("[YMODEM FW] File Size: %d bytes\n", fileSize);

    Serial.printf("[YMODEM FW] Starting OTA update...\n");
    if (!Update.begin(fileSize))
    {
        Serial.printf("[YMODEM FW] ERR: Not enough space for OTA.\n");
        file.close();
        return false;
    }

    Serial.printf("[YMODEM FW] Writing firmware (%d bytes)...\n", fileSize);
    size_t written = Update.writeStream(file);

    if (written != fileSize)
    {
        Serial.printf("[YMODEM FW] ERR: Write error (wrote %d of %d bytes).\n", written, fileSize);
        file.close();
        return false;
    }

    Serial.printf("[YMODEM FW] Finalizing update...\n");
    if (!Update.end())
    {
        Serial.printf("[YMODEM FW] ERR: Finalize update failed.\n");
        file.close();
        return false;
    }

    file.close();

    if (Update.hasError())
    {
        Serial.printf("[YMODEM FW] ERR: Update error code %d.\n", Update.getError());
        return false;
    }

    Serial.printf("[YMODEM FW] Update successful. Rebooting in 5s...\n");
    delay(5000);
    ESP.restart();

    // Clean up the firmware file if needed
    if (SPIFFS.remove(FUOTA_FILE_NAME))
    {
        Serial.printf("[YMODEM FW] Firmware file removed.\n");
    }

    return true;
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

void YModem::sendACK()
{
    this->_timeout = millis();
    //Serial.printf("[YMODEM TX] ACK sent\n");
    uint8_t ack = ACK;
    this->__ymodem_serial.write(&ack, 1);
}

void YModem::sendNAK()
{
    this->_timeout = millis();
    Serial.printf("[YMODEM TX] NAK sent\n");
    uint8_t nak = NAK;
    this->__ymodem_serial.write(&nak, 1);
}

void YModem::sendCRCRequest()
{
    this->_timeout = millis();
    Serial.printf("[YMODEM TX] CRC Request sent\n");
    uint8_t crc16 = CRC16;
    this->__ymodem_serial.write(&crc16, 1);
}

// Calculates the CRC for a given data buffer
uint16_t YModem::calculateCRC(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i] << 8;
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc & 0x8000) ? ((crc << 1) ^ 0x1021) : (crc << 1);
        }
    }
    return crc;
}

void YModem::receivePacket(uint8_t *buffer, uint16_t &size)
{
    // static variables to keep track of the file transfer progress
    static int32_t fileSize = 0;
    static int32_t initialFileSize = 0;
    static File file;

    switch (this->_state)
    {
    case YMODEM_IDLE:
        // Idle state – no processing required.
        break;

    case WAIT_FOR_HEADER:
    {
        if (buffer[0] == SOH || buffer[0] == STX)
        {
            this->_timeout = millis();
            char fileName[128];
            sscanf((char *)&buffer[2], "%127s", fileName);
            sscanf((char *)&buffer[2 + strlen(fileName) + 1], "%lu", &fileSize);
            initialFileSize = fileSize;

            Serial.printf("[YMODEM RX] HDR: '%s' (%ld B)\n", fileName, fileSize);

            file = SPIFFS.open(FUOTA_FILE_NAME, FILE_WRITE, true);
            if (!file)
            {
                Serial.printf("[YMODEM] ERR: Cannot open file for writing\n");
                return;
            }
            memset(ymodem_buffer, 0, sizeof(ymodem_buffer));
            ymodem_buffer_size = 0;
            setState(RECEIVE_DATA);
            sendACK();
            delay(10);
            sendCRCRequest();
        }
    }
    break;

    case RECEIVE_DATA:
    {
        if (buffer[0] == EOT)
        {
            this->_timeout = millis();
            Serial.printf("[YMODEM RX] EOT received. Finalizing...\n");
            sendACK();
            file.close();
            Serial.printf("[YMODEM] FW update initiated.\n");
            update_esp32_firmware();
            setState(YMODEM_IDLE);
        }
        else if (buffer[0] == SOH || buffer[0] == STX)
        {
            this->_timeout = millis();
            uint16_t received_crc = (buffer[size - 2] << 8) | buffer[size - 1];
            uint16_t calculated_crc = calculateCRC(buffer + 3, 1024);

            if (received_crc == calculated_crc)
            {
                uint32_t size_to_write = (fileSize > 1024) ? 1024 : fileSize;
                if (file)
                {
                    file.write(buffer + 3, size_to_write);
                }
                fileSize -= size_to_write;
                int progress = (int)(((initialFileSize - fileSize) * 100) / initialFileSize);
                Serial.printf("File Transfer Progress:\t\t %d %% Completed \n",progress);
                sendACK();
            }
            else
            {
                Serial.printf("[YMODEM RX] ERR: CRC mismatch (rec=0x%04X, calc=0x%04X)\n", received_crc, calculated_crc);
                sendNAK();
            }
        }
    }
    break;

    case WAIT_EOT:
    {
        if (buffer[0] == EOT)
        {
            Serial.printf("[YMODEM RX] EOT confirmed.\n");
            sendACK();
            setState(YMODEM_IDLE);
        }
    }
    break;

    case COMPLETE:
        Serial.printf("[YMODEM] Transfer complete. Resetting.\n");
        setState(YMODEM_IDLE);
        break;
    }
}

void YModem::setState(ymodem_state_t state)
{
    const char* stateNames[] = {"Idle", "Header", "Data", "Wait EOT", "Complete"};
    //Serial.printf("[YMODEM] State changed to: %s\n", stateNames[state]);
    this->_state = state;
}

ymodem_state_t YModem::getState()
{
    // Optionally, you can add a user-friendly log here if needed.
    return this->_state;
}

void YModem::process_timeout()
{
    if (this->_state == YMODEM_IDLE)
        return;
    
    if ((millis() - this->_timeout) > YMODEM_TIMEOUT)
    {
        Serial.printf("[YMODEM] Timeout (%lu ms elapsed) in state %d. Resetting.\n", millis() - this->_timeout, this->_state);
        setState(YMODEM_IDLE);
    }
}