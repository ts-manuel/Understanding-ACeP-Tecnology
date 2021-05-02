/**
 ******************************************************************************
 * @file      common.h
 * @author    ts-manuel
 * @brief     Common functions for command parsers
 *
 ******************************************************************************
 */

#ifndef INC_CMD_COMMON_H_
#define INC_CMD_COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

typedef void (*fp_command)(const char*);

typedef struct{
	fp_command	handler;
	const char*	name;
} Command_t;


const char* CMD_Trim(const char* str, const char* msg);
const char* CMD_TrimSpaces(const char* str);
const char* ReadColor(const char* str, uint8_t* color);

#endif /* INC_CMD_COMMON_H_ */
