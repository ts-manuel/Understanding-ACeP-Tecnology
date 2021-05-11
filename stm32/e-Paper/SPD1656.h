/**
 ******************************************************************************
 * @file      SPD1656.h
 * @author    ts-manuel
 * @brief     Full color ACeP ePaper display driver
 *
 * 			Controller: SPD1656  solomon systech
 * 			FLASH:		W25X20CL winbound
 *
 ******************************************************************************
 */

#ifndef SPD1656_H_
#define SPD1656_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"


#define EPD_WIDTH	600
#define EPD_HEIGHT	448

static const uint8_t color2lut[8] = {0,2,3,1,4,5,6,7};
#define LOAD_VCM_LUT(_lut) 			EPD_LoadLUT(0x20, _lut, sizeof(_lut))
#define LOAD_CLR_LUT(_color, _lut)	EPD_LoadLUT(0x21 + color2lut[(_color)], _lut, sizeof(_lut))
#define LOAD_XON_LUT(_lut) 			EPD_LoadLUT(0x29, _lut, sizeof(_lut))

typedef enum
{
  EPD_OK       = 0x00U,
  EPD_ERROR    = 0x01U,
  EPD_BUSY     = 0x02U,
  EPD_TIMEOUT  = 0x04U
} EPD_Status_t;


EPD_Status_t EPD_Init(void);
bool EPD_IsReady(void);

EPD_Status_t EPD_LoadLUT(uint8_t reg, const uint8_t* lut, int len);

void EPD_BeginUpdate(void);
void EPD_WritePixel(uint8_t color);
EPD_Status_t EPD_EndUpdate(void);

EPD_Status_t EPD_Clear(uint8_t color);
EPD_Status_t EPD_ShowBlocks(void);

void EPD_FlashRead(uint32_t addr, uint8_t* buffer, int len);


#endif /* SPD1656_H_ */
