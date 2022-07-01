#include "main.h"

#ifndef AS5013
#define AS5013

#define AS5013_I2C_ADDRESS (0x40 << 1)
#define AS5013_ID_CODE_REG (0x0c)
#define AS5013_ID_VERSION_REG (0x0d)
#define AS5013_SILICONE_REVISION_REG (0x0e)

#define AS5013_CONTROL_REG_1 (0x0f)
#define AS5013_X_REG (0x10)
#define AS5013_Y_RESET_INT_REG (0x11)
#define AS5013_X_LEFT_THRESHOLD_REG (0x12)
#define AS5013_X_RIGHT_THRESHOLD_REG (0x13)
#define AS5013_Y_TOP_THRESHOLD_REG (0x14)
#define AS5013_Y_BOTTOM_THRESHOLD_REG (0x15)
#define AS5013_M_CONTROL_REG (0x2b)
#define AS5013_J_CONTROL_REG (0x2c)
#define AS5013_SCALING_CONTROL_REG (0x2d)
#define AS5013_CONTROL_REG_2 (0x2e)

/* To be or'd with an internal register address when repeated reads are needed */
#define AS5013_AUTO_INCREMENT_DISABLE (1 << 6)

extern UART_HandleTypeDef huart2;

void AS5013_hello_world(uint8_t *echo);

/**
 * @brief Reads a single byte from a specified register on the AS5013.
 *
 * @param hi2c The handle of the I²C connection the AS5013 is on.
 * @param device_register The AS5013 register to be read.
 * @param data The read value will be written to this byte.
 *
 * @retval HAL_OK on success, HAL_ERROR on any kind of error, HAL_BUSY if the I²C connection is busy.
 */
HAL_StatusTypeDef AS5013_generic_read_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t *read_result);

/**
 * @brief Writes a single byte to a specified register on the AS5013.
 *
 * @param hi2c The handle of the I²C connection the AS5013 is on.
 * @param device_register The AS5013 register to be written to.
 * @param data The data to be written.
 *
 * @retval HAL_OK on success, HAL_ERROR on any kind of error, HAL_BUSY if the I²C connection is busy.
 */
HAL_StatusTypeDef AS5013_generic_write_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t data);

/**
 * @brief Reads the device id register. Hardware specific.
 */
HAL_StatusTypeDef AS5013_get_id_code(I2C_HandleTypeDef *hi2c, uint8_t *read_result);

/**
 * @brief Reads the AS5013 joystick's x component. Note that its format it natively signed two's complement
 * eight bit integer.
 */
HAL_StatusTypeDef AS5013_get_x(I2C_HandleTypeDef *hi2c, int8_t *read_result);

/**
 * @brief Reads the AS5013 joystick's y component. Note that its format it natively signed two's complement
 * eight bit integer.
 */
HAL_StatusTypeDef AS5013_get_y(I2C_HandleTypeDef *hi2c, int8_t *read_result);


#endif
