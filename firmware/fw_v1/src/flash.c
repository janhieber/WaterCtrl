/*
 * flash.c
 *
 *  Created on: 24.04.2015
 *      Author: jan
 */

#include <stm32f0xx.h>
#include <flash.h>


/********************
 * TODO: this should be rewritten from STM32 HAl to StdPeriph Lib
 * simply replace the HAL... function with their coresponding functions in StdPeriph
 * Tip: google how to abuse the flash as storage on STM32
 * Also look at stm32f0xx_flash.c and stm32f0xx_flash.h
 *********************/





/*
void writeConfig(void) {
	// unlock flash
	if (HAL_FLASH_Unlock() != HAL_OK) {
		HAL_FLASH_Lock();
		printf("ERR while unlocking flash\r\n");
		return;
	}
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
			FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR |
			FLASH_FLAG_PGSERR);

	// erase flash
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_11;
	EraseInitStruct.NbSectors = 1;
	uint32_t SectorError = 0;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
		HAL_FLASH_Lock();
		printf("ERR while erasing flash\r\n");
		return;
	}

	// program flash
	uint32_t addr = ADDR_FLASH_SECTOR_11;
	if (strlen(last_config) > 0) {
		// write magic identifier
		if (HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, FLASH_MAGIC) !=
				HAL_OK) {
			HAL_FLASH_Lock();
			printf("ERR while programming flash\r\n");
			return;
		}
		addr += 4;

		uint32_t i = 0;
		while (last_config[i] != 0) {
			if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, addr, last_config[i]) !=
					HAL_OK) {
				HAL_FLASH_Lock();
				printf("ERR while programming flash\r\n");
				return;
			}
			addr++;
			i++;
		}
		// terminate with 0
		if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, addr, 0) != HAL_OK) {
			HAL_FLASH_Lock();
			printf("ERR while programming flash\r\n");
			return;
		}
	}

	// lock flash
	HAL_FLASH_Lock();

	printf("\tcurrent config saved!\r\n");
}
*/


/*
void readConfig(void) {
	uint32_t data;
	uint32_t addr;

	addr = ADDR_FLASH_SECTOR_11;

	// read magic identifier
	data = *((const uint32_t*)addr);
	if (data != FLASH_MAGIC) {
		printf("\tno stored config found...\r\n");
		return;
	}
	addr += 4;

	// check length
	if (strlen((const char *)addr) > USB_BUF_SIZE) {
		printf("\tstored config invalid...\r\n");
		return;
	}

	// restore
	printf("\tstored config found, restoring...\r\n");
	strcpy(last_config, (const char *)addr);
	parseCFG(last_config);
}
*/
