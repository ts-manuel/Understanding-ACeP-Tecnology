/**
 ******************************************************************************
 * @file      LUTs.h
 * @author    ts-manuel
 * @brief     LUT defines
 *
 ******************************************************************************
 */

#ifndef LUTS_H_
#define LUTS_H_

#include <stdint.h>

//VCOM Levels
#define VCM_DC	0	//TFT adjustable negative bias (-1V)
#define VCM_HI	1	//15V + VCM_DC
#define VCM_LO	2	//-15V + VCM_DC
#define VCM_FL	3	//Floating
#define VCM_LV(_l1, _l2, _l3, _l4, _l5, _l6, _l7, _l8) ((_l1)<<6)|((_l2)<<4)|((_l3)<<2)|(_l4), ((_l5)<<6)|((_l6)<<4)|((_l7)<<2)|(_l8)
//PIXEL Levels
#define PIX_GND	0	//0V
#define PIX_VSH	1	//15V
#define PIX_VSL 2	//-15V
#define PIX_LVH 3	//VSH_LV adjustable (10V)
#define PIX_LVL 4	//VSL_LV adjustable (-10V)
#define PIX_LV2 5	//NON used
#define PIX_FLT	7	//Floating
#define PIX_LV(_l1, _l2, _l3, _l4, _l5, _l6, _l7, _l8) ((_l1)<<4)|(_l2), ((_l3)<<4)|(_l4), ((_l5)<<4)|(_l6), ((_l7)<<4)|(_l8)
//XON Levels
#define XON_VGH	0	//All gate ON (VGH)
#define XON_SCN	1	//Normal gate scan function
#define XON_LV(_l1, _l2, _l3, _l4, _l5, _l6, _l7, _l8) ((_l1)<<7)|((_l1)<<6)|((_l1)<<5)|((_l1)<<4)|((_l1)<<3)|((_l1)<<2)|((_l1)<<1)|(_l1)


// Stock LUTs
extern const uint8_t LUT_STOCK_VCOM[12*4];
extern const uint8_t LUT_STOCK_COLOR_0[13*4];
extern const uint8_t LUT_STOCK_COLOR_1[13*5];
extern const uint8_t LUT_STOCK_COLOR_2[13*4];
extern const uint8_t LUT_STOCK_COLOR_3[13*4];
extern const uint8_t LUT_STOCK_COLOR_4[13*5];
extern const uint8_t LUT_STOCK_COLOR_5[13*5];
extern const uint8_t LUT_STOCK_COLOR_6[13*4];
extern const uint8_t LUT_STOCK_COLOR_7[13*5];
extern const uint8_t LUT_STOCK_XON[10*1];

// Self clear with transparent color LUTs
extern const uint8_t LUT_STOCK_SCT_VCOM[11*8];
extern const uint8_t LUT_STOCK_SCT_COLOR_0[13*9];
extern const uint8_t LUT_STOCK_SCT_COLOR_1[13*10];
extern const uint8_t LUT_STOCK_SCT_COLOR_2[13*9];
extern const uint8_t LUT_STOCK_SCT_COLOR_3[13*9];
extern const uint8_t LUT_STOCK_SCT_COLOR_4[13*10];
extern const uint8_t LUT_STOCK_SCT_COLOR_5[13*10];
extern const uint8_t LUT_STOCK_SCT_COLOR_6[13*9];
extern const uint8_t LUT_STOCK_SCT_COLOR_7[13*8];
extern const uint8_t LUT_STOCK_SCT_XON[10*2];

//Positive step
extern const uint8_t LUT_STEP_POS_VCOM[12*1];
extern const uint8_t LUT_STEP_POS[4][8][13];
extern const uint8_t LUT_STEP_POS_XON[11*1];

//Negative step
extern const uint8_t LUT_STEP_NEG_VCOM[12*1];
extern const uint8_t LUT_STEP_NEG[4][8][13];
extern const uint8_t LUT_STEP_NEG_XON[11*1];

#endif /* LUTS_H_ */
