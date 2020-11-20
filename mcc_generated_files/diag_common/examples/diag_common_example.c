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
 *  @file    diag_common_example.c
 *  @brief   This file contains examples to demonstrate diagnostics API usage
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <xc.h>
#include "diag_common_example.h"
#include "../../diag_library/memory/volatile/diag_sram_marchb.h"
#include "../../diag_library/memory/volatile/diag_sram_checkerboard.h"

void DIAG_SRAM_MarchB_Example(void)
{
    if (SRAM_OK == DIAG_SRAM_MarchB_GetStatus())
    {
        printf("\r\nPassed : SRAM March-B test\r\n");
    }
    else
    {
        printf("\r\nFailed : SRAM March-B test\r\n");
    }
}

void DIAG_SRAM_CheckerBoard_Example(void)
{
    if (SRAM_OK == DIAG_SRAM_CheckerBoard((uint8_t*) INTERNAL_SRAM_START, INTERNAL_SRAM_SIZE))
    {
        printf("\r\nPassed : SRAM Checkerboard test\r\n");
    }
    else
    {
        printf("\r\nFailed : SRAM Checkerboard test\r\n");
    }
}

/**
End of File
 */