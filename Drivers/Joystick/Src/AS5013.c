#include "main.h"
#include "AS5013.h"
#include <string.h>

HAL_StatusTypeDef AS5013_generic_read_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t *read_result) {
	if ((NULL == hi2c) || (NULL == read_result))
	{
		return HAL_ERROR;
	}

	HAL_StatusTypeDef write_result = HAL_I2C_Master_Transmit(hi2c, AS5013_I2C_ADDRESS | 0, &device_register, 1, 1000);

	if (HAL_OK != write_result)
	{
		return write_result;
	}

	return HAL_I2C_Master_Receive(hi2c, AS5013_I2C_ADDRESS | 1, read_result, 1, 1000);
}

HAL_StatusTypeDef AS5013_generic_write_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t data) {
	if (NULL == hi2c)
	{
		return HAL_ERROR;
	}

	uint8_t message[] = { device_register, data };

	return HAL_I2C_Master_Transmit(hi2c, AS5013_I2C_ADDRESS | 0, message, 2, 1000);
}

HAL_StatusTypeDef AS5013_get_id_code(I2C_HandleTypeDef *hi2c, uint8_t *read_result) {
	return AS5013_generic_read_single(hi2c, AS5013_ID_CODE_REG, read_result);
}

HAL_StatusTypeDef AS5013_get_x(I2C_HandleTypeDef *hi2c, int8_t *read_result) {
	return AS5013_generic_read_single(hi2c, AS5013_X_REG, (uint8_t *) read_result);
}

HAL_StatusTypeDef AS5013_get_y(I2C_HandleTypeDef *hi2c, int8_t *read_result) {
	return AS5013_generic_read_single(hi2c, AS5013_Y_RESET_INT_REG, (uint8_t *) read_result);
}
