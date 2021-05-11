/**
 ******************************************************************************
 * @file      cmd_ldlut.c
 * @author    ts-manuel
 * @brief     Handles commands received by the serial port
 *
 ******************************************************************************
 */

#include "cmd/parser.h"
#include "cmd/common.h"
#include "SPD1656.h"
#include "LUTs.h"


/*
 * Parse ldlut command
 * Loads the specified LUT
 * */
void CMD_ldlut(const char* args)
{
	EPD_Status_t res = EPD_OK;
	const char *str1, *str2;

	if(strcmp(args, "stock") == 0)
	{
		res |= LOAD_VCM_LUT(LUT_STOCK_VCOM);
		res |= LOAD_CLR_LUT(0, LUT_STOCK_COLOR_0);	//Black
		res |= LOAD_CLR_LUT(1, LUT_STOCK_COLOR_1);	//Blue
		res |= LOAD_CLR_LUT(2, LUT_STOCK_COLOR_2);	//White
		res |= LOAD_CLR_LUT(3, LUT_STOCK_COLOR_3);	//Green
		res |= LOAD_CLR_LUT(4, LUT_STOCK_COLOR_4);	//Red
		res |= LOAD_CLR_LUT(5, LUT_STOCK_COLOR_5);	//Yellow
		res |= LOAD_CLR_LUT(6, LUT_STOCK_COLOR_6);	//Orange
		res |= LOAD_CLR_LUT(7, LUT_STOCK_COLOR_7);	//Clean
		res |= LOAD_XON_LUT(LUT_STOCK_XON);
	}
	else if(strcmp(args, "stock_sct") == 0)
	{
		res |= LOAD_VCM_LUT(LUT_STOCK_SCT_VCOM);
		res |= LOAD_CLR_LUT(0, LUT_STOCK_SCT_COLOR_0);	//Black
		res |= LOAD_CLR_LUT(1, LUT_STOCK_SCT_COLOR_1);    //Blue
		res |= LOAD_CLR_LUT(2, LUT_STOCK_SCT_COLOR_2);    //White
		res |= LOAD_CLR_LUT(3, LUT_STOCK_SCT_COLOR_3);    //Green
		res |= LOAD_CLR_LUT(4, LUT_STOCK_SCT_COLOR_4);    //Red
		res |= LOAD_CLR_LUT(5, LUT_STOCK_SCT_COLOR_5);    //Yellow
		res |= LOAD_CLR_LUT(6, LUT_STOCK_SCT_COLOR_6);    //Orange
		res |= LOAD_CLR_LUT(7, LUT_STOCK_SCT_COLOR_7);    //Transparent
		res |= LOAD_XON_LUT(LUT_STOCK_SCT_XON);
	}
	else if((str1 = CMD_Trim(args, "step")))
	{
		const char* strvlt[4] = {"5", "15", "25", "30"};
		bool pos_step;
		int i;

		if((str2 = CMD_Trim(str1, "pos")))
			pos_step = true;
		else if((str2 = CMD_Trim(str1, "neg")))
			pos_step = false;
		else
		{
			printf("Invalid argument -%s-\n", args);
			return;
		}

		i = CMD_Find(str2, strvlt, 4);

		if(i == -1)
		{
			printf("Invalid argument -%s-\n", args);
			return;
		}

		if(pos_step)
		{
			res |= LOAD_VCM_LUT(LUT_STEP_POS_VCOM);
			res |= LOAD_CLR_LUT(0, LUT_STEP_POS[i][0]);
			res |= LOAD_CLR_LUT(1, LUT_STEP_POS[i][1]);
			res |= LOAD_CLR_LUT(2, LUT_STEP_POS[i][2]);
			res |= LOAD_CLR_LUT(3, LUT_STEP_POS[i][3]);
			res |= LOAD_CLR_LUT(4, LUT_STEP_POS[i][4]);
			res |= LOAD_CLR_LUT(5, LUT_STEP_POS[i][5]);
			res |= LOAD_CLR_LUT(6, LUT_STEP_POS[i][6]);
			res |= LOAD_CLR_LUT(7, LUT_STEP_POS[i][7]);
			res |= LOAD_XON_LUT(LUT_STEP_POS_XON);
		}
		else
		{
			LOAD_VCM_LUT(LUT_STEP_NEG_VCOM);
			LOAD_CLR_LUT(0, LUT_STEP_NEG[i][0]);
			LOAD_CLR_LUT(1, LUT_STEP_NEG[i][1]);
			LOAD_CLR_LUT(2, LUT_STEP_NEG[i][2]);
			LOAD_CLR_LUT(3, LUT_STEP_NEG[i][3]);
			LOAD_CLR_LUT(4, LUT_STEP_NEG[i][4]);
			LOAD_CLR_LUT(5, LUT_STEP_NEG[i][5]);
			LOAD_CLR_LUT(6, LUT_STEP_NEG[i][6]);
			LOAD_CLR_LUT(7, LUT_STEP_NEG[i][7]);
			LOAD_XON_LUT(LUT_STEP_NEG_XON);
		}
	}
	else
	{
		printf("Invalid argument -%s-\n", args);
		return;
	}

	if(res == EPD_OK)
		printf("Done\n");
	else
		printf("Command terminated with ERROR!\n");
}

