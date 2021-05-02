/**
 ******************************************************************************
 * @file      cmd_fread.c
 * @author    ts-manuel
 * @brief     Handles commands received by the serial port
 *
 ******************************************************************************
 */

#include "cmd/parser.h"
#include "SPD1656.h"

#define _BFF_LEN 256

void CMD_fread(const char* args)
{
	uint8_t buff[_BFF_LEN];
	uint32_t start = 0x000000;
	uint32_t stop  = 0x020000;
	uint32_t addr;

	//Extract start and stop addresses
	sscanf(args, "%ld %ld", &start, &stop);
	printf("Reading FLASH from %ld(0x%06lx) to %ld(0x%06lx)\n\n", start, start, stop, stop);

	//Read data
	addr = start;
	while(addr < stop)
	{
		//Read data
		uint16_t len = (stop - addr) < _BFF_LEN ? (stop - addr) : _BFF_LEN;
		EPD_FlashRead(addr, buff, len);

		//Print data
		printf("%06lX: ", addr);
		for(int i = 0; i < len; i++)
		{
			if((i % 16 == 0) && (i != 0))
			{
				printf("\n%06lX: ", addr + i);
			}

			printf("%02X ", buff[i]);
		}
		addr += len;
		printf("\n");
	}

	printf("\nDone\n");
}
