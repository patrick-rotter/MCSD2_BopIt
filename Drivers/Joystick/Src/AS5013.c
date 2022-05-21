#include "main.h"
#include "AS5013.h"
#include <string.h>

void AS5013_hello_world(uint8_t *echo) {
	HAL_UART_Transmit(&huart2, echo, strlen((const char *) echo), 1000);
}


uint8_t AS5013_get_id_code(I2C_HandleTypeDef hi2c) {
	uint8_t retval = 1;

	uint8_t reg = AS5013_ID_CODE_REG;

	if (HAL_I2C_Master_Transmit(&hi2c, AS5013_I2C_ADDRESS | 0, &reg, 1, 1000) != HAL_OK) {
		return 2;
	}

	if (HAL_I2C_Master_Receive(&hi2c, AS5013_I2C_ADDRESS | 1, &retval, 1, 1000) != HAL_OK) {
		return 3;
	}

	return retval;
}
