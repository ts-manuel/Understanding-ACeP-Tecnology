/**
 ******************************************************************************
 * @file      cmd_draw.c
 * @author    ts-manuel
 * @brief     Handles commands received by the serial port
 *
 ******************************************************************************
 */

#include "cmd/parser.h"
#include "cmd/common.h"
#include "SPD1656.h"
#include "stm32f4xx_hal.h"


/*
 * Parse draw command
 * */
void CMD_draw(const char* args)
{
	const char* str_color;
	EPD_Status_t res;

	printf("Wait...\n");

	if((str_color = CMD_Trim(args, "rect")))
	{
		const char* str_pos;
		uint8_t color;
		int x0 = 0, x1 = EPD_WIDTH, y0 = 0, y1 = EPD_HEIGHT;

		//Extract color form string
		str_pos = ReadColor(str_color, &color);

		if(color < 8)
		{
			//Read position
			sscanf(str_pos, "%d %d %d %d", &x0, &y0, &x1, &y1);

			printf("Drawing rectangle at (%d,%d)(%d,%d)\n", x0, y0, x1, y1);

			EPD_BeginUpdate();

			for(int y = 0; y < EPD_HEIGHT; y++)
			{
				for(int x = 0; x < EPD_WIDTH; x++)
				{
					if((y >= y0) && (y < y1) && (x > x0) && (x < x1))
						EPD_WritePixel(color);
					else
						EPD_WritePixel(7);
				}
			}

			res = EPD_EndUpdate();
		}
		else
		{
			printf("Invalid color -%s-\n", str_color);
			return;
		}
	}
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
