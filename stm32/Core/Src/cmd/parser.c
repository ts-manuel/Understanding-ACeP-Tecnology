/**
 ******************************************************************************
 * @file      parser.c
 * @author    ts-manuel
 * @brief     Handles commands received by the serial port
 *
 ******************************************************************************
 */

#include <cmd/parser.h>


//List of commands
void CMD_display(const char* args);
void CMD_fread(const char* args);
void CMD_draw(const char* args);
void CMD_ldlut(const char* args);
const Command_t commands[] =
{
		{CMD_display,	"display"},
		{CMD_fread, 	"fread"},
		{CMD_draw,		"draw"},
		{CMD_ldlut, 	"ldlut"},
};


// Serial RX buffer
static USART_TypeDef* uart_instance;
static volatile char new_char;
static volatile char buffer[_MAX_CMD_LENGTH];
static volatile int rx_read_ptr;
static volatile int rx_write_ptr;
static volatile bool new_char_available = false;
#define RX_AVAILABLE_DATA() (((unsigned int)(rx_write_ptr - rx_read_ptr)) % _MAX_CMD_LENGTH)
#define RX_DATA(x) buffer[(rx_read_ptr + (x)) % _MAX_CMD_LENGTH]
#define RX_REMOVE_CHARS(x) {rx_read_ptr = (rx_read_ptr + (x)) % _MAX_CMD_LENGTH;}


//Function prototypes
static void CMD_Invalid(const char* str);


/*
 * This callback is called by the HAL_UART_IRQHandler
 * when the given number of bytes are received
 * */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == uart_instance)
	{
		//If character is backspace
		if(new_char == 127)
		{
			//Remove last character from buffer
			if(rx_write_ptr != rx_read_ptr)
			{
				rx_write_ptr = (unsigned int)(rx_write_ptr - 1) % _MAX_CMD_LENGTH;

				//Echo character back
				HAL_UART_Transmit(huart, (uint8_t*)&new_char, 1, 100);
			}
		}
		//Enter character if buffer is not full
		else if(RX_AVAILABLE_DATA() < _MAX_CMD_LENGTH-1)
		{
			//Insert byte into circular buffer
			buffer[rx_write_ptr] = new_char;
			rx_write_ptr = (rx_write_ptr + 1) % _MAX_CMD_LENGTH;

			//Echo character back
			HAL_UART_Transmit(huart, (uint8_t*)&new_char, 1, 100);

			new_char_available = true;
		}
		//Ring the bell when the buffer if full
		else
		{
			char bel = '\a';
			HAL_UART_Transmit(huart, (uint8_t*)&bel, 1, 100);
		}

		//Receive next character
		HAL_UART_Receive_IT(huart, (uint8_t*)&new_char, 1);
	}
}


/*
 * Reads (len) characters from the rx buffer and appends a '\0'
 * */
static void ReadStringFromRXBuffer(char* dest, int len)
{
	while(len > 0)
	{
		*dest = buffer[rx_read_ptr];
		len--;
		dest++;
		rx_read_ptr = (rx_read_ptr + 1) % _MAX_CMD_LENGTH;
	}
	*dest = '\0';
}


/*
 * Initialize rx buffer and start reception
 * */
void CMD_Init(UART_HandleTypeDef* huart)
{
	rx_read_ptr = 0;
	rx_write_ptr = 0;
	uart_instance = huart->Instance;
	HAL_UART_Receive_IT(huart, (uint8_t*)&new_char, 1);
}


/*
 * Try to parse command,
 * check for '\n' terminated string in the receive buffer
 * if a valid command is found execute it
 * */
void CMD_TryParse(void)
{
	char str[_MAX_CMD_LENGTH+1];
	const char* cmd;
	const char* args;
	int i = 0;
	bool found;
	bool valid;

	//If there is a new character from the serial port
	if(new_char_available)
	{
		new_char_available = false;

		//Search for '\n' in the receive buffer
		found = false;
		while(!found && i < RX_AVAILABLE_DATA())
		{
			if(RX_DATA(i) == '\n' || RX_DATA(i) == '\r')
			{
				found = true;
			}

			i++;
		}

		//If '\n' is found parse string
		if(found)
		{
			//Read command string into cmd_str
			ReadStringFromRXBuffer(str, i - 1);

			//Remove '\n' '\r' characters
			while(RX_AVAILABLE_DATA() > 0 && (RX_DATA(0) == '\n' || RX_DATA(0) == '\r'))
			{
				RX_REMOVE_CHARS(1);
			}

			cmd = CMD_TrimSpaces(str);

			//Search for valid command at the beginning of the string
			valid = false;
			for(int i = 0; i < sizeof(commands)/sizeof(Command_t) && !valid; i++)
			{
				if((args = CMD_Trim(cmd, commands[i].name)))
				{
					commands[i].handler(args);
					valid = true;
				}
			}

			if(!valid && strlen(str) > 0)
			{
				CMD_Invalid(str);
			}
		}
	}
}


/*
 * Parse invalid command,
 * echo command with error message
 * */
static void CMD_Invalid(const char* str)
{
	printf("Invalid command: -%s-\n", str);
}
