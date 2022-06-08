#include "CY8C201A0.h"
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"

void CY8C201A0_hello_world(char *echo_str) {
	HAL_UART_Transmit(&huart2, (uint8_t*) echo_str, strlen((char *) echo_str), 1000);
}

HAL_StatusTypeDef CY8_generic_write_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t data) {
	if (NULL == hi2c) {
		return HAL_ERROR;
	}

	uint8_t message[] = { device_register, data };

	return HAL_I2C_Master_Transmit(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS | 1, message, 2, 1000);
}

HAL_StatusTypeDef CY8_generic_read_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t *data) {
	if ((NULL == hi2c) || (NULL == data))
	{
		return HAL_ERROR;
	}


	HAL_StatusTypeDef transmit_retval = HAL_I2C_Master_Transmit(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS | 0, &device_register, 1, 1000);

	if (HAL_OK != transmit_retval ) {
		return transmit_retval;
	}

	return HAL_I2C_Master_Receive(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS | 1, data, 1, 1000);
}


HAL_StatusTypeDef CY8_send_command(I2C_HandleTypeDef *hi2c, uint8_t command_code) {

	HAL_StatusTypeDef retval = CY8_generic_write_single(hi2c, CY8C201A0_COMMAND_REG, command_code);

	if (HAL_OK == retval)
	{
		osDelay(CY8C201A0_COMMAND_WAIT_TIME_MS);
	}

	return retval;

}

HAL_StatusTypeDef CY8_set_normal_opmode(I2C_HandleTypeDef *hi2c) {

	return CY8_send_command(hi2c, CY8C201A0_SET_OPMODE_NORMAL_COMMAND);

}

HAL_StatusTypeDef CY8_set_setup_opmode(I2C_HandleTypeDef *hi2c) {

	return CY8_send_command(hi2c, CY8C201A0_SET_OPMODE_SETUP_COMMAND);

}

HAL_StatusTypeDef CY8_store_current_config(I2C_HandleTypeDef *hi2c) {

	return CY8_send_command(hi2c, CY8C201A0_STORE_CONFIG_COMMAND);

}

HAL_StatusTypeDef CY8_init(	I2C_HandleTypeDef *hi2c,
							UART_HandleTypeDef *huart,
							uint8_t capsense_0_config,
							uint8_t capsense_1_config,
							uint8_t slider_pad_number,
							uint8_t resolution_code) {

	(void) huart; /* In case logging is to be added */

	if(HAL_OK != HAL_I2C_IsDeviceReady(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS, 5, 1000))
	{
		/* Sensor not responding to custom address, try and set it */
		if (HAL_OK != HAL_I2C_IsDeviceReady(hi2c, I2C_GENERAL_CALL_ADDRESS, 5, 1000))
		{
			/* Sensor not responding to general call - connection error */
			goto error_handler;
		}

		int error_counter = 0;
		error_counter += CY8_unlock_i2c_reg(hi2c, I2C_GENERAL_CALL_ADDRESS);
		error_counter += CY8_set_i2c_addr(hi2c, I2C_GENERAL_CALL_ADDRESS, CY8C201A0_CUSTOM_I2C_ADDRESS >> 1); /* Shift right to eliminate R/W bit */
		error_counter += CY8_lock_i2c_reg(hi2c, I2C_GENERAL_CALL_ADDRESS);

		error_counter += HAL_I2C_IsDeviceReady(hi2c, CY8C201A0_CUSTOM_I2C_ADDRESS, 5, 1000);

		if (0 != error_counter)
		{
			/* Could not set custom i²c address */
			goto error_handler;
		}
	}

	int error_counter = 0;

	error_counter += CY8_set_setup_opmode(hi2c);

	error_counter += CY8_set_capsense_0_config(hi2c, capsense_0_config);
	error_counter += CY8_set_capsense_1_config(hi2c, capsense_1_config);


	error_counter += CY8_enable_slider(hi2c, slider_pad_number);

	error_counter += CY8_set_slider_resolution(hi2c, resolution_code);

	error_counter += CY8_store_current_config(hi2c);

	error_counter += CY8_set_normal_opmode(hi2c);

	if (0 == error_counter)
	{
		return HAL_OK;
	}

	error_handler:
	return HAL_ERROR;
}

HAL_StatusTypeDef CY8_get_device_ID(I2C_HandleTypeDef *hi2c, uint8_t *id_result) {

	return CY8_generic_read_single(hi2c, CY8C201A0_DEVICE_ID_REG, id_result);

}

HAL_StatusTypeDef CY8_enable_slider(I2C_HandleTypeDef *hi2c, uint8_t slider_pad_number) {
	if (5 == slider_pad_number)
	{
		return CY8_generic_write_single(hi2c, CY8C201A0_CAPSENSE_SLIDER_CONFIG_REG, 0x01);
	}
	else if (10 == slider_pad_number)
	{
		return CY8_generic_write_single(hi2c, CY8C201A0_CAPSENSE_SLIDER_CONFIG_REG, 0x03);
	}
	return HAL_ERROR;

}

HAL_StatusTypeDef CY8_set_slider_resolution(I2C_HandleTypeDef *hi2c, uint8_t resolution_code) {
	return CY8_generic_write_single(hi2c, CY8C201A0_SLIDER_RESOLUTION_0_REG, resolution_code);
}


HAL_StatusTypeDef CY8_unlock_i2c_reg(I2C_HandleTypeDef *hi2c, uint8_t address) {
	uint8_t unlock_seq[4] = {CY8C201A0_I2C_DEV_LOCK_REG, 0x3c, 0xa5, 0x69};

	return HAL_I2C_Master_Transmit(hi2c, address << 1, unlock_seq, 4, 1000);
}

HAL_StatusTypeDef CY8_lock_i2c_reg(I2C_HandleTypeDef *hi2c, uint8_t address) {
	uint8_t lock_seq[4] = {CY8C201A0_I2C_DEV_LOCK_REG, 0x96, 0x5a, 0xc3};

	return HAL_I2C_Master_Transmit(hi2c, address << 1, lock_seq, 4, 1000);
}

HAL_StatusTypeDef CY8_set_i2c_addr(I2C_HandleTypeDef *hi2c, uint8_t old_address, uint8_t new_address) {
	/* The new address cannot be wider than 7 bit */
	if (!is_legal_i2c_addr(new_address)) {
		return HAL_ERROR;
	}

	uint8_t command_seq[2] = { CY8C201A0_I2C_ADDRESS_REG, new_address };

	return HAL_I2C_Master_Transmit(hi2c, old_address << 1, command_seq, 2, 1000);
}

HAL_StatusTypeDef CY8_set_capsense_0_config(I2C_HandleTypeDef *hi2c, uint8_t config) {
	return CY8_generic_write_single(hi2c, CY8C201A0_CAPSENSE_ENABLE_0_REG, config);
}

HAL_StatusTypeDef CY8_set_capsense_1_config(I2C_HandleTypeDef *hi2c, uint8_t config) {
	return CY8_generic_write_single(hi2c, CY8C201A0_CAPSENSE_ENABLE_1_REG, config);
}

HAL_StatusTypeDef CY8_read_capsense_0(I2C_HandleTypeDef *hi2c, uint8_t *read_result) {
	return CY8_generic_read_single(hi2c, CY8C201A0_READ_CAPSENSE_STATUS_0_REG, read_result);
}

HAL_StatusTypeDef CY8_read_capsense_1(I2C_HandleTypeDef *hi2c, uint8_t *read_result) {
	return CY8_generic_read_single(hi2c, CY8C201A0_READ_CAPSENSE_STATUS_1_REG, read_result);
}

int is_legal_i2c_addr(uint8_t address) {
	if ((address > 127) || (address < 10)) {
		return 0;
	}

	return 1;
}
