#include "main.h"
#include "Si1153.h"
#include <string.h>

void Si1153_hello_world(uint8_t *echo) {
	HAL_UART_Transmit(&huart2, echo, strlen((char *) echo), 1000);
}
