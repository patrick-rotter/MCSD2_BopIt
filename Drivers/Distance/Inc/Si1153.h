#include "main.h"

#ifndef SI1153
#define SI1153

#define SI1153_I2C_ADDRESS (0x53 << 1)

#define SI1153_PART_ID_REG (0x00)

extern UART_HandleTypeDef huart2;

void Si1153_hello_world(uint8_t *echo);

uint8_t Si1153_get_part_id(I2C_HandleTypeDef hi2c);

#endif
