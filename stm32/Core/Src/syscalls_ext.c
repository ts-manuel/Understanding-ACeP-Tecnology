/**
 ******************************************************************************
 * @file      syscalls_ext.c
 * @author    ts-manuel
 * @brief     Extension of the System calls file
 *
 ******************************************************************************
 */

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;


#if defined(__GNUC__)

int _write(int fd, char * ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

int _read(int file, char *ptr, int len)
{
	HAL_UART_Receive(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

#elif defined (__ICCARM__)
#include "LowLevelIOInterface.h"

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) buffer, size, HAL_MAX_DELAY);
	return size;
}

#elif defined (__CC_ARM)

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

#endif
