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


#include "../include/mcc.h"

/**
 * Initializes MCU, drivers and middleware in the project
**/

int8_t CLKCTRL_Initialize();

void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    FLASH_Initialize();
    CPUINT_Initialize();
    CLKCTRL_Initialize();
}

/**
 * \brief Initialize CLKCTRL_Initialize interface
 */
int8_t CLKCTRL_Initialize()
{
    //CLKOUT disabled; CLKSEL Internal high-frequency oscillator; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),0x0);
    
    //PDIV 2X; PEN disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),0x0);
    
    //LOCKEN disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0x0);
    
    //PLLS disabled; EXTS disabled; XOSC32KS disabled; OSC32KS disabled; OSCHFS disabled; SOSC disabled; 
    ccp_write_io((void*)&(CLKCTRL.MCLKSTATUS),0x0);
    
    //RUNSTDBY disabled; FREQSEL 4 MHz system clock (default); AUTOTUNE disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSCHFCTRLA),0xC);
    
    //TUNE 0x0; 
    ccp_write_io((void*)&(CLKCTRL.OSCHFTUNE),0x0);
    
    //RUNSTDBY disabled; 
    ccp_write_io((void*)&(CLKCTRL.OSC32KCTRLA),0x0);
    
    //RUNSTDBY disabled; SOURCE disabled; MULFAC PLL is disabled; 
    ccp_write_io((void*)&(CLKCTRL.PLLCTRLA),0x0);
    
    //RUNSTDBY disabled; CSUT 1k cycles; SEL disabled; LPMODE disabled; ENABLE disabled; 
    ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),0x0);
    

    return 0;
}


