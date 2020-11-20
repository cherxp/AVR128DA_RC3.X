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
 *  @file    diag_sram_marchb.h
 *  @brief   This file contains API prototypes for SRAM March-B test
 * 
 *  @note 
 *  Microchip Technology Inc. has followed development methods required by 
 *  IEC-60730 and performed extensive validation and static testing to ensure 
 *  that the code operates as intended. Any modification to the code can 
 *  invalidate the results of Microchip's validation and testing.
 *
 */

#ifndef DIAG_SRAM_MARCHB_H
#define DIAG_SRAM_MARCHB_H

/**
 * @brief This module contains functional safety diagnostics APIs for SRAM March-B
 * @defgroup diag_sram_marchb SRAM - March B Algorithm
 * @brief The following is an UML Interface diagram for March B algorithm: \n
 * @image html marchB-interfaces.png
 * @brief The following is an UML Sequence diagram for March B algorithm: \n
 * @image html marchB-sequence.png
 * @{
 */

#include "diag_sram_types.h"
#include <stdint.h>
#include <xc.h>

/**
 @ingroup diag_sram_marchb
 @def SRAM_SEC_SIZE
 This is a macro for size of each SRAM section
 */
#define SRAM_SEC_SIZE    (16)

/**
 @ingroup diag_sram_marchb
 @def SRAM_NSECS
 This is a macro to configure number of SECTIONS for SRAM
 */
#define SRAM_NSECS       (INTERNAL_SRAM_SIZE / SRAM_SEC_SIZE)

/**
 @ingroup diag_sram_marchb
 @brief This API check the entire SRAM using March-B test.

 Device SRAM starts from INTERNAL_SRAM_START, which is device dependent.
 In order to make it possible to run the test even with application data in SRAM,
 the memory is divided into a configurable number of sections SRAM_NSECS that are tested in turn.
 Therefore, at a time we will have @ref SRAM_SEC_SIZE to be tested with March algorithms
 The first memory section of @ref SRAM_SEC_SIZE length(referred to as the march_buffer) is reserved.
 march_buffer is used by the test to store the content of the other sections while they are being tested.
 With usage of march_buffer, this test is made as non-destructive with periodicity as "On-Startup" or "Periodic"
 For the applications where SRAM usage is more, march_buffer size can be reduced by increasing the size of @ref SRAM_NSECS

 The test behaves as follows:
 - Size of each section in SRAM is @ref SRAM_SEC_SIZE
 - The first section to test is the backup buffer segment, which starts at INTERNAL_SRAM_START and
   ends at INTERNAL_SRAM_START + SRAM_SEC_SIZE.
 - After march_buffer, all subsequent SRAM sections are tested in turn.

 March B test algorithm is executed in below 5 steps:
 Step-1: Ascending(w0) \n
 Step-2: Ascending(r0, w1, r1, w0, r0, w1) \n
 Step-3: Descending(r1, w0, w1) \n
 Step-4: Descending(r1, w0, w1, w0) \n
 Step-5: Descending(r0,w1,w0 ) \n
  
 Error reporting: \n
     @ref DIAG_SRAM_MarchB_GetStatus() should be called from main() to know
     the status of SRAM March-B test
 
 @return See @ref DIAG_SRAM_MarchB_GetStatus()
 */
void DIAG_SRAM_MarchB(void);

/**
 @ingroup diag_sram_marchb
 @brief This API returns the status of SRAM MarchB check diagnosis
 
 @return @ref SRAM_OK \n
 @ref SRAM_ERROR \n 
*/
diag_sram_status_t DIAG_SRAM_MarchB_GetStatus(void);

/**
 * @}
 */
#endif //DIAG_SRAM_MARCHB_H
