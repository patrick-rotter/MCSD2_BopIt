// I2C 1
#include "main.h"

#ifndef CY8C201A0
#define CY8C201A0

#define I2C_GENERAL_CALL_ADDRESS 0

#define CY8C201A0_CUSTOM_I2C_ADDRESS (0x23 << 1)

#define CY8C201A0_INPUT_0_REG 0x00
#define CY8C201A0_INPUT_1_REG 0x01

/* Enable standalone touchpads as touchsense pads */
#define CY8C201A0_CAPSENSE_ENABLE_0_REG 0x06
/* Enable slider touchpads as touchsense pads */
#define CY8C201A0_CAPSENSE_ENABLE_1_REG 0x07

/* Enable slider pads and determine their number (5 or 10) */
#define CY8C201A0_CAPSENSE_SLIDER_CONFIG_REG 0x75

#define CY8C201A0_SLIDER_RESOLUTION_0_REG 0x77
#define CY8C201A0_SLIDER_RESOLUTION_1_REG 0x78

#define CY8C201A0_I2C_DEV_LOCK_REG 0x79

#define CY8C201A0_DEVICE_ID_REG 0x7a

/* To write to 0x7c, register 0x79 must be unlocked. Changes to 0x7c will only be applied after 0x79 is locked again */
#define CY8C201A0_I2C_ADDRESS_REG 0x7c

#define CY8C201A0_COMMAND_REG 0xa0

#define CY8C201A0_STORE_CONFIG_COMMAND 0x01
#define CY8C201A0_FACTORY_RESTORE_COMMAND 0x02
#define CY8C201A0_RECONFIG_DEVICE_COMMAND 0x06
#define CY8C201A0_SET_OPMODE_NORMAL_COMMAND 0x07
#define CY8C201A0_SET_OPMODE_SETUP_COMMAND 0x08

extern UART_HandleTypeDef huart2;

HAL_StatusTypeDef CY8_generic_write_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t data);
HAL_StatusTypeDef CY8_generic_read_single(I2C_HandleTypeDef *hi2c, uint8_t device_register, uint8_t *data);

HAL_StatusTypeDef CY8_send_command(I2C_HandleTypeDef *hi2c, uint8_t command_code);

/**
 * @brief Performs a single read on the device's id register (0x7a).
 */
HAL_StatusTypeDef CY8_get_device_ID(I2C_HandleTypeDef *hi2c, uint8_t *id_result);

HAL_StatusTypeDef CY8_enable_slider(I2C_HandleTypeDef *hi2c, uint8_t slider_pad_number);

HAL_StatusTypeDef CY8_set_slider_resolution(I2C_HandleTypeDef *hi2c, uint8_t resolution_code);

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
