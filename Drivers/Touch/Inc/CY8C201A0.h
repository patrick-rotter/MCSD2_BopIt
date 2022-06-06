// I2C 1
#include "main.h"

#ifndef CY8C201A0
#define CY8C201A0

#define I2C_GENERAL_CALL_ADDRESS 0

#define CY8C201A0_CUSTOM_I2C_ADDRESS (0x23 << 1)

#define CY8C201A0_DEVICE_ID_REG 0x7a

#define CY8C201A0_I2C_DEV_LOCK_REG 0x79

/* To write to 0x7c, register 0x79 must be unlocked. Changes to 0x7c will only be applied after 0x79 is locked again */
#define CY8C201A0_I2C_ADDRESS_REG 0x7c

extern UART_HandleTypeDef huart2;

/**
 * @brief Performs a single read on the device's id register (0x7a).
 */
HAL_StatusTypeDef CY8_get_device_ID(I2C_HandleTypeDef *hi2c, uint8_t *id_result);

/**
 * @brief Transmits the control sequence that unlocks the CY8C201A0's i²c address register.
 */
HAL_StatusTypeDef CY8_unlock_i2c_reg(I2C_HandleTypeDef hi2c, uint8_t address);

/**
 * @brief Transmits the control sequence that locks the CY8C201A0's i²c address register.
 */
HAL_StatusTypeDef CY8_lock_i2c_reg(I2C_HandleTypeDef hi2c, uint8_t address);

/**
 * @brief Writes the device's new i²c address to the appropriate register. This function should only be called after
 * CY8_unlock_i2c_reg(). Its results will only take effect after calling CY8_lock_i2c_reg().
 */
HAL_StatusTypeDef CY8_set_i2c_addr(I2C_HandleTypeDef hi2c, uint8_t old_address, uint8_t new_address);

void CY8C201A0_hello_world(char *echo_str);

/**
 * @brief Performs a very crude check: is the given address between 10 and 127?
 *
 * @retval 1 if legal, 0 if not
 */
int is_legal_i2c_addr(uint8_t address);


#endif
