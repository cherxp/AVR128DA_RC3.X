/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
*/
/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

#ifndef NVMCTRL_BASIC_H_INCLUDED
#define NVMCTRL_BASIC_H_INCLUDED

#include "../include/utils/compiler.h"

#include <string.h>
#include "ccp.h"

#define BOOTLOADER_SECTION __attribute__((section(".bootloader")))

/** Datatype for flash address */
typedef uint32_t flash_adr_t;

/** Datatype for EEPROM address */
typedef uint16_t eeprom_adr_t;

/** Datatype for return status of NVMCTRL operations */
typedef enum {
    NVM_OK    = 0, ///< NVMCTRL free, no write ongoing.
    NVM_ERROR = 1, ///< NVMCTRL operation retsulted in error
    NVM_BUSY  = 2, ///< NVMCTRL busy, write ongoing.
} nvmctrl_status_t;



int8_t FLASH_Initialize(void);

uint8_t FLASH_ReadEepromByte(eeprom_adr_t eeprom_adr);

nvmctrl_status_t FLASH_WriteEepromByte(eeprom_adr_t eeprom_adr, uint8_t data);

void FLASH_ReadEepromBlock(eeprom_adr_t eeprom_adr, uint8_t *data, size_t size);

nvmctrl_status_t FLASH_WriteEepromBlock(eeprom_adr_t eeprom_adr, uint8_t *data, size_t size);

bool FLASH_IsEepromReady(void);

uint8_t FLASH_ReadFlashByte(flash_adr_t flash_adr);

nvmctrl_status_t FLASH_WriteFlashByte(flash_adr_t flash_adr, uint8_t *ram_buffer, uint8_t data);

nvmctrl_status_t FLASH_EraseFlashPage(flash_adr_t flash_adr);

nvmctrl_status_t FLASH_WriteFlashPage(flash_adr_t flash_adr, uint8_t *data);

nvmctrl_status_t FLASH_WriteFlashBlock(flash_adr_t flash_adr, uint8_t *data, size_t size, uint8_t *ram_buffer);

nvmctrl_status_t FLASH_WriteFlashStream(flash_adr_t flash_adr, uint8_t data, bool finalize);

#endif /* NVMCTRL_BASIC_H_INCLUDED */
