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
 *  @file    diag_sram_checkerboard.h
 *  @brief   This file contains API prototypes for SRAM Checkerboard algorithm
 * 
 *  @note 
 *  Microchip Technology Inc. has followed development methods required by 
 *  IEC-60730 and performed extensive validation and static testing to ensure 
 *  that the code operates as intended. Any modification to the code can 
 *  invalidate the results of Microchip's validation and testing.
 *
 */

#ifndef DIAG_SRAM_CHECKERBOARD_H
#define DIAG_SRAM_CHECKERBOARD_H

/**
 * @brief This module contains functional safety diagnostics APIs for SRAM Checkerboard Test
 * @defgroup diag_sram_checkerboard SRAM - Checkerboard Algorithm
 * @brief The following is an UML Interface diagram for Checkerboard Test algorithm: \n
 * @image html checkerboard-interfaces.png
 * @brief The following is an UML Sequence diagram for Checkerboard Test algorithm: \n
 * @image html checkerboard-sequence.png
 * @{
 */

#include "diag_sram_types.h"
#include <stdint.h>

/**
 @ingroup diag_sram_checkerboard
 @def SRAM_SEC_SIZE
 This is a macro for size of each SRAM section
 */
#define SRAM_SEC_SIZE    (16)

/**
 @ingroup diag_sram_checkerboard
 @brief  This API check the entire SRAM using CheckerBoard test.
 
 Device SRAM starts from INTERNAL_SRAM_START, which is device dependent.
 In order to make it possible to run the test even with application data in SRAM,
 the memory is divided into a configurable number of sections SRAM_NSECS that are tested in turn.
 Therefore, at a time we will have @ref SRAM_SEC_SIZE to be tested with test algorithms
 The first memory section of @ref SRAM_SEC_SIZE length(referred to as the checkerbrd_buffer) is reserved.
 checkerbrd_buffer is used by the test to store the content of the other sections while they are being tested.
 With usage of checkerbrd_buffer, this test is made as non-destructive with periodicity as "On-Startup" or "Periodic"
 For the applications where SRAM usage is more, checkerbrd_buffer size can be reduced by increasing the size of @ref SRAM_NSECS

 The test behaves as follows:
 - Size of each section in SRAM is @ref SRAM_SEC_SIZE
 - The first section to test is the backup buffer segment, which starts at CHECKERBOARD_BUFFER_OFFSET and
   ends at CHECKERBOARD_BUFFER_OFFSET + SRAM_SEC_SIZE.
 - After checkerbrd_buffer, all subsequent SRAM sections are tested in turn.

 CheckerBoard algorithm is executed in below 4 steps:
 Step-1: Write checkerboard with up addressing order \n
 Step-2: Read checkerboard with up addressing order \n
 Step-3: Write inverse checkerboard with up addressing order \n
 Step-4: Read inverse checkerboard with up addressing order \n

 @return @ref SRAM_OK \n 
 @ref SRAM_ERROR \n
 */
diag_sram_status_t DIAG_SRAM_CheckerBoard(register volatile uint8_t* startAddress, register volatile uint16_t length);

/**
 * @}
 */
#endif //DIAG_SRAM_CHECKERBOARD_H
