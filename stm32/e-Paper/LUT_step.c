/**
 ******************************************************************************
 * @file      LUT_step.c
 * @author    ts-manuel
 * @brief     Drives the display with a step voltage
 *
 ******************************************************************************
 */

#include "LUTs.h"


/**
 * This set of waveforms applies a positive step voltage
 * with amplitude V (5 15 25 30) for x (2 4 8 16 32 64 128 255) frames
 *
 *
 * +30V |                            _____       __________              ^^
 * +25V |                           |     |     |          |             ||
 * +15V |                           |     |     |          |           ******
 *  +5V |                           |     |     |          |          |  ||  |
 *   0V |--------------------+  ^^  |     |     |          |          |  vv  +--------------------
 *  -5V |                    |  ||  |     |     |          |          |
 * -15V |                     ******      |     |          |          |
 * -25V |                       ||        |     |          |          |
 * -30V |                       vv        |_____|          |__________|
 *
 *       <------------------> <----> <---> <---> <--------> <--------> <----> <------------------>
 * Frames:     255-x            x     50    50      100        100       x         255-x
 * Voltage:		0              -V    +30   -30      +30        -30       V          0
 *
 * */

const uint8_t LUT_STEP_POS_VCOM[12*1] = {
	1, VCM_LV(VCM_LO, VCM_HI, VCM_LO, VCM_HI, VCM_LO, VCM_HI, VCM_DC, VCM_DC),255, 50, 50,100,100,255,  0,  0,
};

#define _STEP_LUT_POS(_x, _LVL1, _LVL2) 1, PIX_LV(PIX_VSL, (_LVL1), PIX_VSL, PIX_VSH, PIX_VSL, PIX_VSH, (_LVL2), PIX_VSH),255-(_x), (_x), 50, 50,100,100,(_x),255-(_x)

const uint8_t LUT_STEP_POS[4][8][13] = {
	{
		//5V
		{_STEP_LUT_POS(  2, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS(  4, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS(  8, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS( 16, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS( 32, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS( 64, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS(128, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_POS(255, PIX_LVL, PIX_LVH)}
	},{
		//15V
		{_STEP_LUT_POS(  2, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS(  4, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS(  8, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS( 16, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS( 32, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS( 64, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS(128, PIX_GND, PIX_GND)},
		{_STEP_LUT_POS(255, PIX_GND, PIX_GND)}
	},{
		//25V
		{_STEP_LUT_POS(  2, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS(  4, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS(  8, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS( 16, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS( 32, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS( 64, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS(128, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_POS(255, PIX_LVH, PIX_LVL)}
	},{
		//30V
		{_STEP_LUT_POS(  2, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS(  4, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS(  8, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS( 16, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS( 32, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS( 64, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS(128, PIX_VSH, PIX_VSL)},
		{_STEP_LUT_POS(255, PIX_VSH, PIX_VSL)}
	}
};

const uint8_t LUT_STEP_POS_XON[11*1] = {
	1, XON_LV(XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_VGH), 255,255,255,  45,  0,  0,  0,  0,
};



/**
 * This set of waveforms applies a negative step voltage
 * with amplitude V (-5 -15 -25 -30) for x (2 4 8 16 32 64 128 255) frames
 *
 *
 * +30V |                       ^^         _____            __________
 * +25V |                       ||        |     |          |          |
 * +15V |                     ******      |     |          |          |
 *  +5V |                    |  ||  |     |     |          |          |
 *   0V |--------------------+  vv  |     |     |          |          |  ^^  +--------------------
 *  -5V |                           |     |     |          |          |  ||  |
 * -15V |                           |     |     |          |           ******
 * -25V |                           |     |     |          |             ||
 * -30V |                           |_____|     |__________|             vv
 *
 *       <------------------> <----> <---> <---> <--------> <--------> <----> <------------------>
 * Frames:     255-x            x     50    50      100        100       x         255-x
 * Voltage:		0               V    -30   +30      -30        +30      -V          0
 *
 * */

const uint8_t LUT_STEP_NEG_VCOM[12*1] = {
	1, VCM_LV(VCM_HI, VCM_LO, VCM_HI, VCM_LO, VCM_HI, VCM_LO, VCM_DC, VCM_DC),255, 50, 50,100,100,255,  0,  0,
};

#define _STEP_LUT_NEG(_x, _LVL1, _LVL2) 1, PIX_LV(PIX_VSH, (_LVL1), PIX_VSH, PIX_VSL, PIX_VSH, PIX_VSL, (_LVL2), PIX_VSL),255-(_x), (_x), 50, 50,100,100,(_x),255-(_x)

const uint8_t LUT_STEP_NEG[4][8][13] = {
	{
		//5V
		{_STEP_LUT_NEG(  2, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG(  4, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG(  8, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG( 16, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG( 32, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG( 64, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG(128, PIX_LVH, PIX_LVL)},
		{_STEP_LUT_NEG(255, PIX_LVH, PIX_LVL)}
	},{
		//15V
		{_STEP_LUT_NEG(  2, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG(  4, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG(  8, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG( 16, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG( 32, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG( 64, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG(128, PIX_GND, PIX_GND)},
		{_STEP_LUT_NEG(255, PIX_GND, PIX_GND)}
	},{
		//25V
		{_STEP_LUT_NEG(  2, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG(  4, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG(  8, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG( 16, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG( 32, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG( 64, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG(128, PIX_LVL, PIX_LVH)},
		{_STEP_LUT_NEG(255, PIX_LVL, PIX_LVH)}
	},{
		//30V
		{_STEP_LUT_NEG(  2, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG(  4, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG(  8, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG( 16, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG( 32, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG( 64, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG(128, PIX_VSL, PIX_VSH)},
		{_STEP_LUT_NEG(255, PIX_VSL, PIX_VSH)}
	}
};

const uint8_t LUT_STEP_NEG_XON[11*1] = {
	1, XON_LV(XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_SCN, XON_VGH), 255,255,255,  45,  0,  0,  0,  0,
};

