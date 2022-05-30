#include "main.h"

#ifndef AS5013
#define AS5013

#define AS5013_I2C_ADDRESS (0x40 << 1)
#define AS5013_ID_CODE_REG (0x0c)
#define AS5013_ID_VERSION_REG (0x0d)
#define AS5013_SILICONE_REVISION_REG (0x0e)
#define AS5013_CONTROL_REG_1 (0x0f)


/* To be or'd with an internal register address when repeated reads are needed */
#define AS5013_AUTO_INCREMENT_DISABLE (1 << 6)

extern UART_HandleTypeDef huart2;

void AS5013_hello_world(uint8_t *echo);

uint8_t AS5013_get_id_code(I2C_HandleTypeDef hi2c);



#endif
