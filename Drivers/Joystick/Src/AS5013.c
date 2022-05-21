#include "main.h"
#include "AS5013.h"
#include <string.h>

void AS5013_hello_world(uint8_t *echo) {
	HAL_UART_Transmit(&huart2, echo, strlen((const char *) echo), 1000);
}
