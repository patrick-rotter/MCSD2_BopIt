#include "CY8C201A0.h"
#include <string.h>
#include <stdint.h>
#include "main.h"

void CY8C201A0_hello_world(char *echo_str) {
	HAL_UART_Transmit(&huart2, (uint8_t*) echo_str, strlen((char *) echo_str), 1000);
}

uint8_t CY8_get_device_ID(I2C_HandleTypeDef hi2c) {

	uint8_t device_register = CY8C201A0_DEVICE_ID_REG;


	// The i2c master transmit function returns on line 1168, while waiting for the TXIS flag to be set
	if (HAL_I2C_Master_Transmit(&hi2c, CY8C201A0_I2C_ADDRESS | 0 , &device_register, 1, 0) == HAL_ERROR) {
		return 1;
	}

	uint8_t id = 3;

	if (HAL_I2C_Master_Receive(&hi2c, CY8C201A0_I2C_ADDRESS | 1 , &id, 1, 0) == HAL_ERROR) {
		return 2;
	}

	return id;
}
