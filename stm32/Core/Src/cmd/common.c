/**
 ******************************************************************************
 * @file      common.c
 * @author    ts-manuel
 * @brief     Common functions for command parsers
 *
 ******************************************************************************
 */

#include "cmd/common.h"


/*
 * If msg appears at the beginning of the string, remove msg + following spaces
 * and return pointer to the following part.
 * Else return NULL
 * */
const char* CMD_Trim(const char* str, const char* msg)
{
	if(str != NULL && strstr(str, msg))
	{
		return CMD_TrimSpaces(str + strlen(msg));
	}
	else
	{
		return NULL;
	}
}


/*
 * Remove spaces from the beginning of the string
 * */
const char* CMD_TrimSpaces(const char* str)
{
	while(*str == ' ')
		str++;

	return str;
}


/*
 * Read a color name from string
 * */
const char* ReadColor(const char* str, uint8_t* color)
{
	const char* str1;

	//Search for color
	if((str1 = CMD_Trim(str, "black")))
		*color = 0;
	else if((str1 = CMD_Trim(str, "blue")))
		*color = 1;
	else if((str1 = CMD_Trim(str, "white")))
		*color = 2;
	else if((str1 = CMD_Trim(str, "green")))
		*color = 3;
	else if((str1 = CMD_Trim(str, "red")))
		*color = 4;
	else if((str1 = CMD_Trim(str, "yellow")))
		*color = 5;
	else if((str1 = CMD_Trim(str, "orange")))
		*color = 6;
	else if((str1 = CMD_Trim(str, "clean")))
		*color = 7;
	else if((str1 = CMD_Trim(str, "lut0")))
		*color = 0;
	else if((str1 = CMD_Trim(str, "lut1")))
		*color = 1;
	else if((str1 = CMD_Trim(str, "lut2")))
		*color = 2;
	else if((str1 = CMD_Trim(str, "lut3")))
		*color = 3;
	else if((str1 = CMD_Trim(str, "lut4")))
		*color = 4;
	else if((str1 = CMD_Trim(str, "lut5")))
		*color = 5;
	else if((str1 = CMD_Trim(str, "lut6")))
		*color = 6;
	else if((str1 = CMD_Trim(str, "lut7")))
		*color = 7;
	else
		*color = 8;

	return CMD_TrimSpaces(str1);
}
