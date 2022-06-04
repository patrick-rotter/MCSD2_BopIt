// I2C 1
#include "main.h"

#ifndef CY8C201A0
#define CY8C201A0

#define CY8C201A0_I2C_ADDRESS (0 << 1)
#define CY8C201A0_DEVICE_ID_REG 0x7a

#define CY8C201A0_I2C_DEV_LOCK_REG 0x79

/* To write to 0x7c, register 0x79 must be unlocked. Changes to 0x7c will only be applied after 0x79 is locked again */
#define CY8C201A0_I2C_ADDRESS_REG 0x7c

extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;

uint8_t CY8_get_device_ID(I2C_HandleTypeDef hi2c);

uint8_t CY8_unlock_i2c_reg(I2C_HandleTypeDef hi2c);
uint8_t CY8_lock_i2c_reg(I2C_HandleTypeDef hi2c);

uint8_t CY8_set_i2c_addr(I2C_HandleTypeDef hi2c, uint8_t new_address);

void CY8C201A0_hello_world(char *echo_str);

/* Not an actual serious check*/
int is_legal_i2c_addr(uint8_t address);


#endif
