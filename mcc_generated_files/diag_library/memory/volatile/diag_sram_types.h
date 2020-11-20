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
 *  @file    diag_sram_types.h
 *  @brief   This file contains common type definitions for SRAM diagnostics module
 * 
 *  @note 
 *  Microchip Technology Inc. has followed development methods required by 
 *  IEC-60730 and performed extensive validation and static testing to ensure 
 *  that the code operates as intended. Any modification to the code can 
 *  invalidate the results of Microchip's validation and testing.
 *
 */

#ifndef DIAG_SRAM_TYPES_H
#define DIAG_SRAM_TYPES_H

/**
 @enum diag_sram_status_t
 @brief This enumeration contains return codes for SRAM diagnostics tests
 @var diag_sram_status_t:: SRAM_OK
 0 - indicates that SRAM test is successful \n
 @var diag_sram_status_t:: SRAM_ERROR
 1 - indicates that SRAM test is unsuccessful \n
 */
typedef enum
{
    SRAM_OK = 0,
    SRAM_ERROR = 1
} diag_sram_status_t;

#endif //DIAG_SRAM_TYPES_H
