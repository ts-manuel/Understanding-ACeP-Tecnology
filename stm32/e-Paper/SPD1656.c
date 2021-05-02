/**
 ******************************************************************************
 * @file      SPD1656.c
 * @author    ts-manuel
 * @brief     Full color ACeP ePaper display driver
 *
 * 			Controller: SPD1656  solomon systech
 * 			FLASH:		W25X20CL winbound
 *
 ******************************************************************************
 */

#include "SPD1656.h"
#include "LUT_test.h"

extern SPI_HandleTypeDef hspi1;

//IO defines
#define EPD_RST_PIN     EP_RST_GPIO_Port, EP_RST_Pin
#define EPD_DC_PIN      EP_DC_GPIO_Port, EP_DC_Pin
#define EPD_CS_PIN      EP_CS_GPIO_Port, EP_CS_Pin
#define EPD_BUSY_PIN    EP_BUSY_GPIO_Port, EP_BUSY_Pin
#define EPD_FCS_PIN		EP_FCS_GPIO_Port, EP_FCS_Pin

//Macros
#define DIGITAL_WRITE(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define DIGITAL_READ(_pin) HAL_GPIO_ReadPin(_pin)
#define DELAY_MS(_x) HAL_Delay((_x))
#define MILLIS() HAL_GetTick()

//Static functions
static void EPD_Reset(void);
static void EPD_Command(uint8_t x);
static void EPD_Data(uint8_t x);
static EPD_Status_t EPD_BusyWait(uint32_t timeout_ms);
static EPD_Status_t EPD_LoadLUT(uint8_t reg, const uint8_t* lut, int len);


#define LOAD_VCM_LUT(_lut) 			EPD_LoadLUT(0x20, _lut, sizeof(_lut))
#define LOAD_CLR_LUT(_color, _lut)	EPD_LoadLUT(0x21 + (_color), _lut, sizeof(_lut))
#define LOAD_XON_LUT(_lut) 			EPD_LoadLUT(0x29, _lut, sizeof(_lut))


//Static variables
static bool Initialized = false;
static uint8_t pix_cache;
static int pix_in_cache = 0;


/*
 * Initializes display controller
 * */
EPD_Status_t EPD_Init(void)
{
	//Reset controller
	EPD_Reset();

	//Wait for BUSY to go HIGH
	if(EPD_BusyWait(100) != EPD_OK)
		return EPD_TIMEOUT;

	//PanelSetting Register (PSR)
	EPD_Command(0x00);
	EPD_Data(0xEF);		//Res=600x448, Gate Scan Direction=Down, Booster Switch=ON, Soft Reset=Normal Operation
#if 0
	EPD_Data(0x00);	//LUTs from flash
#else
	EPD_Data(0x80);	//LUTs from internal register
#endif

	//Power Setting (PWR)
	EPD_Command(0x01);
	EPD_Data(0x37);
	EPD_Data(0x00);	//Gate Voltage Level=20V
	EPD_Data(0x23);	//VSHC_LVL=10V
	EPD_Data(0x23);	//VSLC_LVL=-10V
	EPD_Data(0x00);	//VSLC_VL2 disabled

	//Power OFF Sequence Setting (PFS)
	EPD_Command(0x03);
	EPD_Data(0x00);

	//Booster Soft Start (BTST)
	EPD_Command(0x06);
	EPD_Data(0xC7);
	EPD_Data(0xC7);
	EPD_Data(0x1D);

	/*
	 * When LUTs from FLASH are used, PLL, VCOMDC, VSCHC, VSCHL are read from FLASH
	 * */
	//PLL Control (PLL)
	EPD_Command(0x30);
	EPD_Data(0x3C);			//Frame Rate=50Hz

	//VCOM_DC Setting (VDCS)
	EPD_Command(0x82);
	EPD_Data(1050 / 50);	//data = VCOM(mV) / 50

#if 0
	//Stock
	LOAD_VCM_LUT(LUT_VCOM);
	LOAD_CLR_LUT(0, LUT_COLOR_0);	//Black
	LOAD_CLR_LUT(1, LUT_COLOR_1);	//White
	LOAD_CLR_LUT(2, LUT_COLOR_2);	//Green
	LOAD_CLR_LUT(3, LUT_COLOR_3);	//Blue
	LOAD_CLR_LUT(4, LUT_COLOR_4);	//Red
	LOAD_CLR_LUT(5, LUT_COLOR_5);	//Yellow
	LOAD_CLR_LUT(6, LUT_COLOR_6);	//Orange
	LOAD_CLR_LUT(7, LUT_COLOR_7);	//Clean
	LOAD_XON_LUT(LUT_XON);
#else
	//Self clear with transparent color
	LOAD_VCM_LUT(LUT_VCOM_SC);
	LOAD_CLR_LUT(0, LUT_COLOR_0_SC);	//Black
	LOAD_CLR_LUT(1, LUT_COLOR_1_SC);    //White
	LOAD_CLR_LUT(2, LUT_COLOR_2_SC);    //Green
	LOAD_CLR_LUT(3, LUT_COLOR_3_SC);    //Blue
	LOAD_CLR_LUT(4, LUT_COLOR_4_SC);    //Red
	LOAD_CLR_LUT(5, LUT_COLOR_5_SC);    //Yellow
	LOAD_CLR_LUT(6, LUT_COLOR_6_SC);    //Orange
	LOAD_CLR_LUT(7, LUT_COLOR_7_SC);    //Transparent
	LOAD_XON_LUT(LUT_XON_SC);
#endif

	Initialized = true;
	return EPD_OK;
}


/*
 * Returns true if the display is initialized and ready to receive pixel data
 * */
bool EPD_IsReady(void)
{
	return DIGITAL_READ(EPD_BUSY_PIN) && Initialized;
}


/*
 * Starts the update sequence,
 * must be called prior to send pixel data to the display
 * */
void EPD_BeginUpdate(void)
{
	pix_in_cache = 0;

	//Resolution Setting
	EPD_Command(0x61);
	EPD_Data(0x02);
	EPD_Data(0x58);
	EPD_Data(0x01);
	EPD_Data(0xC0);

	//Data transmission start DTM1
	EPD_Command(0x10);
}


/*
 * Sends pixels to the display,
 * must be called after EPD_BeginUpdate()
 * */
void EPD_WritePixel(uint8_t color)
{
	if(pix_in_cache < 1)
	{
		pix_cache = color;
		pix_in_cache++;
	}
	else
	{
		EPD_Data((pix_cache << 4) | color);
		pix_in_cache = 0;
	}
}


/*
 * Terminates the update sequence and refreshes the display,
 * must be called after all pixels are sent with EPD_WritePixel(color)
 * */
EPD_Status_t EPD_EndUpdate(void)
{
	EPD_Status_t res = EPD_OK;

	//Send the last pixel
	if(pix_in_cache > 0)
	{
		EPD_Data((pix_cache << 4) | 7);
		pix_in_cache = 0;
	}

	//Power ON
	EPD_Command(0x04);
	res |= EPD_BusyWait(100);

	//Display refresh
	EPD_Command(0x12);
	res |= EPD_BusyWait(40000);

	//Power OFF
	EPD_Command(0x02);
	res |= EPD_BusyWait(100);

	return res;
}


/*
 * Clear the screen with the specified color
 * */
EPD_Status_t EPD_Clear(uint8_t color)
{
	//Start data transmission
	EPD_BeginUpdate();

    //Send pixels
    for(int i = 0; i < EPD_HEIGHT*EPD_WIDTH; i++)
    {
    	EPD_WritePixel(color);
    }

    //Refresh display
    return EPD_EndUpdate();
}


/*
 * Show all 8 colors test pattern
 * */
EPD_Status_t EPD_ShowBlocks(void)
{
	//Start data transmission
	EPD_BeginUpdate();

	//Send data
	for(int i = 0; i < EPD_HEIGHT/2; i++)
    {
        for(uint8_t k = 0; k < 4; k++)
        {
            for(int j = 0; j < EPD_WIDTH/4; j++)
            {
                EPD_WritePixel(k);
            }
        }
    }
    for(int i = 0; i < EPD_HEIGHT/2; i++)
    {
        for(uint8_t k = 4; k < 8; k++)
        {
            for(int j = 0; j < EPD_WIDTH/4; j++)
            {
                EPD_WritePixel(k);
            }
        }
    }

    //Refresh display
    return EPD_EndUpdate();
}


/*
 * Reads from the LUT flash memory
 * */
void EPD_FlashRead(uint32_t addr, uint8_t* buffer, int len)
{
	uint8_t cmd[5];

	//Bypass FLASH SPI
	EPD_Command(0x65);
	EPD_Data(0x01);

	cmd[0] = 0x0b;					//Fast Read
	cmd[1] = (addr >> 16) & 0xff;	//24bit Start address
	cmd[2] = (addr >> 8) & 0xff;
	cmd[3] = addr & 0xff;
	cmd[4] = 0;						//Dummy byte

	DIGITAL_WRITE(EPD_FCS_PIN, 0);
	HAL_SPI_Transmit(&hspi1, cmd, 5, 10);
	HAL_SPI_Receive(&hspi1, buffer, len, len);
	DIGITAL_WRITE(EPD_FCS_PIN, 1);

	//Return to normal operation
	EPD_Command(0x65);
	EPD_Data(0x00);
}


/*
 * Load Look-Up Table pad with zeros if required
 * */
static EPD_Status_t EPD_LoadLUT(uint8_t reg, const uint8_t* lut, int len)
{
	int pad;

	//Check validity of input data
	if(reg < 0x20 || reg > 0x29)
		return EPD_ERROR;

	//Calculate the required padding
	if(reg == 0x20)
		pad = 220 - len;
	else if(reg == 0x29)
		pad = 200 - len;
	else
		pad = 260 -len;

	//Return with error if padding is negative
	if(pad < 0)
		return EPD_ERROR;

	//Send data
	EPD_Command(reg);

	for(int i = 0; i < len; i++)
		EPD_Data(lut[i]);

	for(int i = 0; i < pad; i++)
			EPD_Data(0x00);

	return EPD_OK;
}


/*
 * Reset display controller
 * All registers will reset to default value
 * */
static void EPD_Reset(void)
{
	DIGITAL_WRITE(EPD_CS_PIN, 1);
	DIGITAL_WRITE(EPD_FCS_PIN, 1);

	//Reset Sequence
	DIGITAL_WRITE(EPD_RST_PIN, 1);
	DELAY_MS(10);
    DIGITAL_WRITE(EPD_RST_PIN, 0);
    DELAY_MS(1);
    DIGITAL_WRITE(EPD_RST_PIN, 1);
    DELAY_MS(1);

    Initialized = false;
}


/*
 * Write command to display controller
 * */
static void EPD_Command(uint8_t x)
{
	DIGITAL_WRITE(EPD_DC_PIN, 0);
	DIGITAL_WRITE(EPD_CS_PIN, 0);
	HAL_SPI_Transmit(&hspi1, &x, 1, 10);
	DIGITAL_WRITE(EPD_CS_PIN, 1);
}


/*
 * Write data to display controller
 * */
static void EPD_Data(uint8_t x)
{
	DIGITAL_WRITE(EPD_DC_PIN, 1);
	DIGITAL_WRITE(EPD_CS_PIN, 0);
	HAL_SPI_Transmit(&hspi1, &x, 1, 10);
	DIGITAL_WRITE(EPD_CS_PIN, 1);
}


/*
 * Wait for the BUSY pin to go HIGH
 * */
static EPD_Status_t EPD_BusyWait(uint32_t timeout_ms)
{
	uint32_t tickstart = MILLIS();

	while(!DIGITAL_READ(EPD_BUSY_PIN))
	{
		if((MILLIS() - tickstart) >=  timeout_ms)
			return EPD_TIMEOUT;
	}

	return EPD_OK;
}
