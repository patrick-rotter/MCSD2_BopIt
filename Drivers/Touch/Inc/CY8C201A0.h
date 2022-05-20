// I2C 1
#include "main.h"

#ifndef CY8C201A0
#define CY8C201A0

#define CY8C201A0_I2C_ADDRESS (0 << 1)
#define CY8C201A0_DEVICE_ID_REG 0x7a
#define CY8C201A0_I2C_ADDRESS_REG 0x7c

extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;

uint8_t CY8_get_device_ID(I2C_HandleTypeDef hi2c);

void CY8C201A0_hello_world(char *echo_str);



#endif
