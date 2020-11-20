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
 *  @file    diag_sram_marchb.c
 *  @brief   This file contains APIs to test SRAM using March-B algorithm
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
#include "diag_sram_marchb.h"
#include "../../../diag_common/config/diag_config.h"

/**
 @ingroup diag_sram_marchb
 @brief This is the buffer to save contents of SRAM section being tested with March-C algorithm
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

volatile uint8_t march_buffer[SRAM_SEC_SIZE] __at(0x800000 + MARCH_BUFFER_OFFSET);

static volatile __persistent diag_sram_status_t diag_sram_marchb_state;

void DIAG_SRAM_MarchB(void)
{
    register uint8_t *p_sram;
    register uint16_t i = 0, nSec = 0;

    //First test the march_buffer - first section of SRAM is reserved for march_buffer
    //Later test each subsequent SRAM sections - all remaining sections will be backed up and tested
    for (nSec = 0; nSec < SRAM_NSECS; nSec++)
    {
        p_sram = (uint8_t*) (INTERNAL_SRAM_START + (SRAM_SEC_SIZE * nSec));

        //Save content of the current section before running March-B test, unless we are testing the march_buffer itself
        if (p_sram != (uint8_t*) march_buffer)
        {
            for (i = 0; i < SRAM_SEC_SIZE; i++)
            {
                march_buffer[i] = *(p_sram + i);
            }

            //Check that the saved content is not corrupted
            for (i = 0; i < SRAM_SEC_SIZE; i++)
            {
                if (march_buffer[i] != *(p_sram + i))
                {
                    diag_sram_marchb_state = SRAM_ERROR;
                    return;
                }
            }
        }

        //Step-1: Any order - taken as ascending in this case
        //Write 0 to all bit locations
        for (i = 0; i < SRAM_SEC_SIZE; i++)
        {
            *(p_sram + i) = 0x00;
        }

        //Step-2: Ascending -  Read 0, Write 1; Read 1, Write 0; Read 0, Write 1
        //Read a bit and verify that it is 0. If it is 1, a fault has occurred
        //If read as 0, write 1 to its location
        //Read the bit and verify it is 1. If it is 0, a fault has occurred
        //If read as 1, write 0 to its location
        //Read the bit and verify that it is 0. If it is 1, fault has occurred
        //If read as 0, write 1 to its location
        //Repeat the same process for the next bit
        for (i = 0; i < SRAM_SEC_SIZE; i++)
        {
            //Read 0, Write 1
            if (*(p_sram + i) != 0x0)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + i) = 0xFF;
            }

            //Read 1, Write 0
            if (*(p_sram + i) != 0xFF)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + i) = 0x0;
            }

            //Read 0, Write 1
            if (*(p_sram + i) != 0x0)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + i) = 0xFF;
            }
        }

        //Step-3: Ascending - Read 1, Write 0; Write 1
        //Read a bit and verify that it is 1. If it is 0, a fault has occurred.
        //If read as 1, write 0 to its location
        //Write 1 to its location
        //Repeat the same process for the next bit
        for (i = 0; i < SRAM_SEC_SIZE; i++)
        {
            if (*(p_sram + i) != 0xFF)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + i) = 0x0;
                *(p_sram + i) = 0xFF;
            }
        }

        //Step-4: Descending - Read 1, Write 0, Write 1, Write 0
        //Read a bit and verify that it is 1. If its is 0, fault has occurred
        //If read as 1, write 0 to its location
        //Write 1 to its location
        //Write 0 to its location
        //Repeat the same process for the next bit
        for (i = SRAM_SEC_SIZE; i > 0; i--)
        {
            if (*(p_sram + (i - 1)) != 0xFF)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + (i - 1)) = 0;
                *(p_sram + (i - 1)) = 1;
                *(p_sram + (i - 1)) = 0;
            }
        }

        //Step-5: Descending - Read 0, Write 1, Write 0
        //Read a bit and verify that it is 0. If its is 1, fault has occurred
        //If read as 0, write 1 to its location
        //Write 0 to its location
        //Repeat the same process for the next bit
        for (i = SRAM_SEC_SIZE; i > 0; i--)
        {
            if (*(p_sram + (i - 1)) != 0x0)
            {
                diag_sram_marchb_state = SRAM_ERROR;
                return;
            }
            else
            {
                *(p_sram + (i - 1)) = 1;
                *(p_sram + (i - 1)) = 0;
            }
        }

        //Restore the contents of current SRAM section from march_buffer, unless we are testing the march_buffer itself
        if (p_sram != (uint8_t*) march_buffer)
        {
            for (i = 0; i < SRAM_SEC_SIZE; i++)
            {
                *(p_sram + i) = march_buffer[i];
            }

            //Check that the restored content is not corrupted
            for (i = 0; i < SRAM_SEC_SIZE; i++)
            {
                if (*(p_sram + i) != march_buffer[i])
                {
                    diag_sram_marchb_state = SRAM_ERROR;
                    return;
                }
            }
        }
    }

    diag_sram_marchb_state = SRAM_OK;
}

diag_sram_status_t DIAG_SRAM_MarchB_GetStatus(void)
{
    return diag_sram_marchb_state;
}