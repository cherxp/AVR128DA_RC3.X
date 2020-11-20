/**
 *  (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 *  Subject to your compliance with these terms, you may use Microchip software
 *  and any derivatives exclusively with Microchip products. You're responsible
 *  for complying with 3rd party license terms applicable to your use of 3rd
 *  party software (including open source software) that may accompany Microchip
 *  software.
 *
 *  SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 *  APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 *  MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 *  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 *  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
 *  HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
 *  THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 *  CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY,
 *  YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 *  @file    diag_sram_checkerboard.c
 *  @brief   This file contains APIs to test SRAM using Checkerboard algorithm
 *
 *  @note
 *  Microchip Technology Inc. has followed development methods required by
 *  IEC-60730 and performed extensive validation and static testing to ensure
 *  that the code operates as intended. Any modification to the code can
 *  invalidate the results of Microchip's validation and testing.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "diag_sram_checkerboard.h"
#include "../../../diag_common/config/diag_config.h"

/**
 @ingroup diag_sram_checkerboard
 @brief This is the buffer to save contents of SRAM section being tested with CheckerBoard algorithm
 First @ref SRAM_SEC_SIZE bytes of SRAM are reserved for backup buffer
 The address specified for data memory objects must be 0x800000 plus the SRAM start address
 Also .data section should be shifted by SRAM_SEC_SIZE to avoid any overlapping
 For example,
 In case of ATtiny1617, for SRAM_SEC_SIZE of 16, we should shift .data section by 16 bytes, by adding
 "-Wl,--section-start,.data=0x803810" in <em> Project Properties -> XC8 Linker -> Linker Additional Options </em>

 In case of ATtiny817, for SRAM_SEC_SIZE of 16, we should shift .data section by 16 bytes, by adding
 "-Wl,--section-start,.data=0x803E10" in <em> Project Properties -> XC8 Linker -> Linker Additional Options </em>
 
 @note If Checkerboard and March tests are included in the project together, .data section should be offset by 2*SRAM_SEC_SIZE
 */

volatile uint8_t checkerbrd_buffer[SRAM_SEC_SIZE] __at(0x800000 + CHECKERBOARD_BUFFER_OFFSET);

static diag_sram_status_t checkboardTest(register uint8_t* address, register uint8_t size)
{
    register uint8_t i;

    if (address != (uint8_t*) checkerbrd_buffer)
    {
        for (i = 0; i < size; i++)
        {
            checkerbrd_buffer[i] = *(address + i);
        }

        //Check that the saved content is not corrupted
        for (i = 0; i < size; i++)
        {
            if (checkerbrd_buffer[i] != *(address + i))
            {
                return SRAM_ERROR;
            }
        }
    }

    //Write checkerboard with up addressing order
    for (i = 0; i < size; i = i + 2)
    {
        *(address + i) = 0xAA;
        *(address + i + 1) = 0x55;
    }

    //Read checkerboard with up addressing order
    for (i = 0; i < size; i = i + 2)
    {
        if ((*(address + i) != 0xAA) && (*(address + i + 1) != 0x55))
        {
            return SRAM_ERROR;
        }
    }

    //Write inverse checkerboard with up addressing order
    for (i = 0; i < size; i = i + 2)
    {
        *(address + i) = 0x55;
        *(address + i + 1) = 0xAA;
    }

    //Read inverse checkerboard with up addressing order
    for (i = 0; i < size; i = i + 2)
    {
        if ((*(address + i) != 0x55) && (*(address + i + 1) != 0xAA))
        {
            return SRAM_ERROR;
        }
    }

    //Restore the contents of current SRAM section from checkerbrd_buffer, unless we are testing the checkerbrd_buffer itself
    if (address != (uint8_t*) checkerbrd_buffer)
    {

        for (i = 0; i < size; i++)
        {
            *(address + i) = checkerbrd_buffer[i];
        }

        //Check that the restored content is not corrupted
        for (i = 0; i < size; i++)
        {
            if (*(address + i) != checkerbrd_buffer[i])
            {
                return SRAM_ERROR;
            }
        }
    }
    return SRAM_OK;
}

diag_sram_status_t DIAG_SRAM_CheckerBoard(register volatile uint8_t* startAddress, register volatile uint16_t length)
{
    register uint8_t *p_sram;
    register uint16_t nSec = 0;
    register uint16_t sections = length / SRAM_SEC_SIZE;
    register uint8_t remainder = length % SRAM_SEC_SIZE;

    //Backup GIE status
    register bool gieStatus = SREG | CPU_I_bm;

    if ((startAddress < (uint8_t*) INTERNAL_SRAM_START) ||
            (startAddress > (uint8_t*) (INTERNAL_SRAM_START + INTERNAL_SRAM_SIZE)) ||
            (length == 0) ||
            (length > INTERNAL_SRAM_SIZE)
            )
        return SRAM_ERROR;

    //Disable global interrupts during this test
    SREG &= (~CPU_I_bm);

    //Save content of the current section before running SRAM CheckerBoard test
    for (nSec = 0; nSec < sections; nSec++)
    {
        p_sram = (uint8_t*) (startAddress + (SRAM_SEC_SIZE * nSec));

        if (SRAM_ERROR == checkboardTest(p_sram, SRAM_SEC_SIZE))
        {
            //Restore global interrupt enable bit status
            SREG |= (gieStatus << CPU_I_bp);
            return SRAM_ERROR;
        }
    }

    if (remainder)
    {
        p_sram = (uint8_t*) (startAddress + (SRAM_SEC_SIZE * (sections - 1)));

        if (SRAM_ERROR == checkboardTest(p_sram, remainder))
        {
            //Restore global interrupt enable bit status
            SREG |= (gieStatus << CPU_I_bp);
            return SRAM_ERROR;
        }
    }

    //Restore global interrupt enable bit status
    SREG |= (gieStatus << CPU_I_bp);
    return SRAM_OK;
}
