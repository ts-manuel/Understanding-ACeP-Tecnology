/**
 ******************************************************************************
 * @file      cmd_display.c
 * @author    ts-manuel
 * @brief     Handles commands received by the serial port
 *
 ******************************************************************************
 */

#include "cmd/parser.h"
#include "cmd/common.h"
#include "SPD1656.h"



/*
 * Parse display command
 * */
void CMD_display(const char* args)
{
	EPD_Status_t res;

	printf("Wait...\n");

	if(strcmp(args, "blocks") == 0)
		res = EPD_ShowBlocks();
	else
	{
		uint8_t color;

		if(ReadColor(args, &color) != NULL)
		{
			res = EPD_Clear(color);
		}
		else
		{
			printf("Invalid argument -%s-\n", args);
			return;
		}
	}

	if(res == EPD_OK)
		printf("Done\n");
	else
		printf("Command terminated with ERROR!\n");
}

