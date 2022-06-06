#include "CY8C201A0.h"
#include <string.h>
#include <stdint.h>
#include "main.h"

void CY8C201A0_hello_world(char *echo_str) {
	HAL_UART_Transmit(&huart2, (uint8_t*) echo_str, strlen((char *) echo_str), 1000);
}

HAL_StatusTypeDef CY8_get_device_ID(I2C_HandleTypeDef *hi2c, uint8_t *id_result) {

	if (id_result == NULL) {
		return HAL_ERROR;
	}

	uint8_t device_register = CY8C201A0_DEVICE_ID_REG;


	// The i2c master transmit function returns on line 1168, while waiting for the TXIS flag to be set
	if (HAL_I2C_Master_Transmit(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS | 0 , &device_register, 1, 1000) == HAL_ERROR) {
		return HAL_ERROR;
	}


	return HAL_I2C_Master_Receive(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS | 1 , id_result, 1, 1000);

}


HAL_StatusTypeDef CY8_unlock_i2c_reg(I2C_HandleTypeDef hi2c, uint8_t address) {
	uint8_t unlock_seq[4] = {CY8C201A0_I2C_DEV_LOCK_REG, 0x3c, 0xa5, 0x69};

	return HAL_I2C_Master_Transmit(&hi2c, address, unlock_seq, 4, 1000);
}

HAL_StatusTypeDef CY8_lock_i2c_reg(I2C_HandleTypeDef hi2c, uint8_t address) {
	uint8_t lock_seq[4] = {CY8C201A0_I2C_DEV_LOCK_REG, 0x96, 0x5a, 0xc3};

	return HAL_I2C_Master_Transmit(&hi2c, address, lock_seq, 4, 1000);
}

HAL_StatusTypeDef CY8_set_i2c_addr(I2C_HandleTypeDef hi2c, uint8_t old_address, uint8_t new_address) {
	/* The new address cannot be wider than 7 bit */
	if (!is_legal_i2c_addr(new_address)) {
		return HAL_ERROR;
	}

	uint8_t command_seq[2] = { CY8C201A0_I2C_ADDRESS_REG, new_address };

	return HAL_I2C_Master_Transmit(&hi2c, old_address, command_seq, 2, 1000);
}

int is_legal_i2c_addr(uint8_t address) {
	if ((address > 127) || (address < 10)) {
		return 0;
	}

	return 1;
}
