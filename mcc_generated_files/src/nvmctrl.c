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

#include "../include/nvmctrl.h"
#include <avr/pgmspace.h>

/**
 * \brief Initialize nvmctrl interface
 * \return Return value 0 if success
 */
int8_t FLASH_Initialize(void)
{
    //FLMAPLOCK disabled; FLMAP SECTION0; APPDATAWP disabled; BOOTRP disabled; APPCODEWP disabled; 
    NVMCTRL.CTRLB = 0;

    //EEREADY disabled; 
    NVMCTRL.INTCTRL = 0;

    return 0;
}

ISR(NVMCTRL_EE_vect)
{

    /* The interrupt flag has to be cleared manually */
    NVMCTRL.INTFLAGS = NVMCTRL_EEREADY_bm;
}

/**
 * \brief Read a byte from eeprom
 *
 * \param[in] eeprom_adr The byte-address in eeprom to read from
 *
 * \return The read byte
 */
uint8_t FLASH_ReadEepromByte(eeprom_adr_t eeprom_adr)
{
		// Read operation will be stalled by hardware if any write is in progress		
		return *(uint8_t *)(EEPROM_START + eeprom_adr);
	
}

/**
 * \brief Write a byte to eeprom
 *
 * \param[in] eeprom_adr The byte-address in eeprom to write to
 * \param[in] data The byte to write
 *
 * \return Status of write operation
 */
nvmctrl_status_t FLASH_WriteEepromByte(eeprom_adr_t eeprom_adr, uint8_t data)
{
		/* Wait for completion of previous write */
		while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));

		/* Program the EEPROM with desired value(s) */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_EEERWR_gc);

		/* Write byte to EEPROM */
		*(uint8_t *)(EEPROM_START + eeprom_adr) = data;
		
		/* Clear the current command */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc); 

		return NVM_OK;		
}

/**
 * \brief Read a block from eeprom
 *
 * \param[in] eeprom_adr The byte-address in eeprom to read from
 * \param[in] data Buffer to place read data into
 *
 * \return Nothing
 */
void FLASH_ReadEepromBlock(eeprom_adr_t eeprom_adr, uint8_t *data, size_t size)
{
		// Read operation will be stalled by hardware if any write is in progress
		memcpy(data, (uint8_t *)(EEPROM_START + eeprom_adr), size);
	
}

/**
 * \brief Write a block to eeprom
 *
 * \param[in] eeprom_adr The byte-address in eeprom to write to
 * \param[in] data The buffer to write
 *
 * \return Status of write operation
 */
nvmctrl_status_t FLASH_WriteEepromBlock(eeprom_adr_t eeprom_adr, uint8_t *data, size_t size)
{
		uint8_t *write = (uint8_t *)(EEPROM_START + eeprom_adr);

		/* Wait for completion of previous write */
		while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));
		/* Program the EEPROM with desired value(s) */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_EEERWR_gc);

		do {
			/* Write byte to EEPROM */
			*write++ = *data++;
			size--;
		} while (size != 0);

		/* Clear the current command */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

		return NVM_OK;
}

/**
 * \brief Check if the EEPROM can accept data to be read or written
 *
 * \return The status of EEPROM busy check
 * \retval false The EEPROM can not receive data to be read or written
 * \retval true The EEPROM can receive data to be read or written
 */
bool FLASH_IsEepromReady(void)
{
		return (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));
}

void FLASH_SpmWriteWord(uint32_t address, uint16_t word)
{
		__asm__ __volatile__                        \
		(      
			"push r0\n\t"                           /* back up R0*/\
			"lds r0, %0\n\t"						/* back up RAMPZ*/\
			"push r0\n\t"							/* back up RAMPZ*/\
			"movw r0, %A2\n\t"                      /* update R0,R1 pair with word*/\
			"movw r30, %A1\n\t"                     /* update R30,R31 pair with address[Byte1,Byte0]*/\
			"sts  %0, %C1\n\t"                      /* update RAMPZ with address[Byte2]*/\
			"spm \n\t"                              /* spm */\
			"clr r1\n\t"                            /* R1 is always assumed to be zero by the compiler. Resetting R1 to zero*/\
			"pop r0\n\t"							/* restore RAMPZ*/\
			"sts %0, r0\n\t"						/* restore RAMPZ*/\
			"pop r0 \n\t"                           /* restore R0*/\
			:                                       \
			: "i" (_SFR_MEM_ADDR(RAMPZ)),           \
			"r" ((uint32_t)(address)),              \
			"r" ((uint16_t)(word))					\
			: "r30", "r31"							/* Clobber R30, R31 to indicate they are used here*/\
		);	
}


/**
 * \brief Read a byte from flash
 *
 * \param[in] flash_adr The byte-address in flash to read from
 *
 * \return The read byte
 */
uint8_t FLASH_ReadFlashByte(flash_adr_t flash_adr)
{
	return pgm_read_byte_far(flash_adr);
}

/**
 * \brief Write a byte to flash
 *
 * \param[in] flash_adr The byte-address in flash to write to
 * \param[in] page_buffer A buffer in memory the size of a flash page, used as a scratchpad
 * \param[in] data The byte to write
 *
 * \return Status of the operation
 */
nvmctrl_status_t FLASH_WriteFlashByte(flash_adr_t flash_adr, uint8_t *ram_buffer, uint8_t data)
{
	flash_adr_t start_of_page = (flash_adr_t)(flash_adr & ~((flash_adr_t)PROGMEM_PAGE_SIZE - 1));
	uint16_t *word_buffer = (uint16_t *)ram_buffer;
	uint16_t i;

	/* Backup all the FLASH page data to ram_buffer and update the new data*/
	for (i = 0; i < flash_adr % PROGMEM_PAGE_SIZE; i++) {
			ram_buffer[i] = pgm_read_byte_far(start_of_page+i);
	}
	ram_buffer[i++] = data;
	for (; i < PROGMEM_PAGE_SIZE; i++) {
			ram_buffer[i] = pgm_read_byte_far(start_of_page+i);
	}

	/* Wait for completion of previous operation */
	while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));

	/* Erase the flash page */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLPER_gc);
	/* dummy write to start erase operation */
	FLASH_SpmWriteWord(start_of_page,0);

	/* Wait for completion of previous operation */
	while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));

	/*A change from one command to another must always go through NOCMD or NOOP*/
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

	/* Program the page with desired value(s) */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLWR_gc);

	/* write the modified page data to FLASH*/
	for (i = 0; i < PROGMEM_PAGE_SIZE/2; i++) {	
		FLASH_SpmWriteWord(start_of_page+(i*2),word_buffer[i]);
	}
	/* Clear the current command */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

	if (NVMCTRL.STATUS & NVMCTRL_ERROR_gm)
		return NVM_ERROR;
	else
		return NVM_OK;
}

/**
 * \brief Erase a page in flash
 *
 * \param[in] flash_adr The byte-address in flash to erase. Must point to start-of-page.
 *
 * \return Status of the operation
 */
nvmctrl_status_t FLASH_EraseFlashPage(flash_adr_t flash_adr)
{
	/* Wait for completion of previous operation */
	while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm));

	/* Erase the flash page */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLPER_gc);

	/* dummy write to start erase operation */
	FLASH_SpmWriteWord(flash_adr,0);

	/* Clear the current command */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

	if (NVMCTRL.STATUS & NVMCTRL_ERROR_gm)
		return NVM_ERROR;
	else
		return NVM_OK;
}


/**
 * \brief Write a page in flash. No page erase is performed by this function.
 *
 * \param[in] flash_adr: starting address of NVM page which needs to be written
 * \param[in] data: pointer to an array of size 'PROGMEM_PAGE_SIZE'
 *
 * \return Status of the operation
 */
nvmctrl_status_t FLASH_WriteFlashPage(flash_adr_t flash_adr, uint8_t *data)
{
	uint16_t *word_buffer = (uint16_t *)data;
	
	/* check for the starting address of page*/
	if (flash_adr % PROGMEM_PAGE_SIZE != 0) {
		return NVM_ERROR;
	}

	/* Wait for completion of previous operation */
	while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm))
		;

	/* Erase the flash page */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLPER_gc);

	/* Perform a dummy write to this address to update the address register in NVMCTL */
	FLASH_SpmWriteWord(flash_adr,0);

	/* Wait for completion of previous operation */
	while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm))
		;

	/*A change from one command to another must always go through NOCMD or NOOP*/
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

	/* Write the flash page */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLWR_gc);

	/* Write data to the page buffer */
	for (uint16_t i = 0; i < PROGMEM_PAGE_SIZE/2; i++) {
		FLASH_SpmWriteWord(flash_adr+(i*2),word_buffer[i]);
	}	

	/* Clear the current command */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);

	if (NVMCTRL.STATUS & NVMCTRL_ERROR_gm)
		return NVM_ERROR;
	else
		return NVM_OK;
}

/**
 * \brief Writes a buffer to flash.
 * The flash does not need to be erased beforehand.
 * The flash address to write to does not need to be aligned to any specific boundary.
 *
 * \param[in] flash_adr The byte-address of the flash to write to
 * \param[in] data The data to write to the flash
 * \param[in] size The size of the data (in bytes) to write to the flash
 * \param[in] page_buffer A buffer in memory the size of a flash page, used as a scratchpad
 *
 * \return Status of the operation
 */
nvmctrl_status_t FLASH_WriteFlashBlock(flash_adr_t flash_adr, uint8_t *data, size_t size, uint8_t *ram_buffer)
{
	flash_adr_t      data_space   = (flash_adr & ~((flash_adr_t)PROGMEM_PAGE_SIZE - 1));
	uint32_t         start_offset = flash_adr % PROGMEM_PAGE_SIZE;
	uint16_t         i;
	nvmctrl_status_t status = NVM_ERROR;

	// Step 1:
	// Fill page buffer with contents of first flash page to be written up
	// to the first flash address to be replaced by the new contents
	for (i = 0; i < start_offset; i++) {
		ram_buffer[i] = FLASH_ReadFlashByte(data_space+i);
	}

	// Step 2:
	// Write all of the new flash contents to the page buffer, writing the
	// page buffer to flash every time the buffer contains a complete flash
	// page.
	while (size > 0) {
		ram_buffer[i++] = *data++;
		size--;
		if ((i % PROGMEM_PAGE_SIZE) == 0) {
			status = FLASH_WriteFlashPage(data_space, ram_buffer);
			i      = 0;
			data_space += PROGMEM_PAGE_SIZE;
			if (status == NVM_ERROR) {
				return NVM_ERROR;
			}
		}
	}

	// Step 3:
	// After step 2, the page buffer may be partially full with the last
	// part of the new data to write to flash. The remainder of the flash page
	// shall be unaltered. Fill up the remainder
	// of the page buffer with the original contents of the flash page, and do a
	// final flash page write.
	while (i) {
		ram_buffer[i] = FLASH_ReadFlashByte(data_space+i);
		i++;
		if ((i % PROGMEM_PAGE_SIZE) == 0) {
			status = FLASH_WriteFlashPage(data_space, ram_buffer);
			i      = 0;
			data_space += PROGMEM_PAGE_SIZE;
		}
	}

	return status;
}

/**
 * \brief Writes a byte stream to flash.
 * The erase granularity of the flash (i.e. one page) will cause this operation
 * to erase an entire page at a time. To avoid corrupting other flash contents,
 * make sure that the memory range in flash being streamed to is starting on a page
 * boundary, and that enough flash pages are available to hold all data being written.
 *
 * The function will perform flash page operations such as erase and write
 * as appropriate, typically when the last byte in a page is written. If
 * the last byte written is not at the last address of a page, the "finalize"
 * parameter can be set to force a page write after this byte.
 *
 * This function is intended used in devices where RAM resources are too limited
 * to afford a buffer needed by the write and erase page functions, and where
 * performance needs and code size concerns leaves the byte write and block
 * write functions too expensive.
 *
 * \param[in] flash_adr The byte-address of the flash to write to
 * \param[in] data The data byte to write to the flash
 * \param[in] finalize Set to true for the final write to the buffer
 *
 * \return Status of the operation
 */
nvmctrl_status_t FLASH_WriteFlashStream(flash_adr_t flash_adr, uint8_t data, bool finalize)
{
	static bool first_byte = true;
	uint16_t word_data;
	
	if (first_byte) {
		/* check for the starting of page */
		if ( flash_adr % PROGMEM_PAGE_SIZE != 0) {
			return NVM_ERROR;
		}
		first_byte = false;
	}
	/* check for a new page */
	if (flash_adr % PROGMEM_PAGE_SIZE == 0) {
		/* Erase the flash page and program with desired value(s) */
		/* Wait for completion of previous operation */
		while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm))
			;

		/* Erase the flash page */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLPER_gc);
		/* dummy write to start erase operation */
		FLASH_SpmWriteWord(flash_adr,0);

		/* Wait for completion of previous operation */
		while (NVMCTRL.STATUS & (NVMCTRL_EEBUSY_bm|NVMCTRL_FBUSY_bm))
			;

		/* Program the page with desired value(s) */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_FLWR_gc);
	}
	
	if ( flash_adr %2)
		word_data = data << 8 | 0xFF;
	else
		word_data = 0xFF << 8 | data;
		
	// Write the new word value to the correct address. Making the flash_adr word aligned
	FLASH_SpmWriteWord(flash_adr & ~(flash_adr_t)1,word_data);

	if (finalize) {
		/* Clear the current command */
		ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
		first_byte = true;
	}

	if (NVMCTRL.STATUS & NVMCTRL_ERROR_gm)
		return NVM_ERROR;
	else
		return NVM_OK;
}
