#include "CY8C201A0.h"
#include <string.h>
#include <stdint.h>
#include "main.h"

void CY8C201A0_hello_world(char *echo_str) {
	HAL_UART_Transmit(&huart2, (uint8_t*) echo_str, strlen((char *) echo_str), 1000);
}
