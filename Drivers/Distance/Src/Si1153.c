#include "main.h"
#include "Si1153.h"
#include <string.h>

void Si1153_hello_world(uint8_t *echo) {
	HAL_UART_Transmit(&huart2, echo, strlen((char *) echo), 1000);
}

uint8_t Si1153_get_part_id(I2C_HandleTypeDef hi2c) {
	uint8_t retval = 0;

	uint8_t reg_addr = SI1153_PART_ID_REG;

	if(HAL_I2C_Master_Transmit(&hi2c, SI1153_I2C_ADDRESS | 0, &reg_addr, 1, 1000) != HAL_OK) {
		return 1;
	}

	if (HAL_I2C_Master_Receive(&hi2c, SI1153_I2C_ADDRESS | 1, &retval, 1, 1000) != HAL_OK) {
		return 2;
	}

	return retval;
}
